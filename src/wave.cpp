// #include <iostream>
#include "constants.hpp"
#include "physics.hpp"
#include "sea.hpp"
#include "wave.hpp"

void fixedUpdateWaves(Sea& sea){
	//Update the wave for this time step
	Wave* waves = sea.waves;
	for(short i = 0; i < sea.numWaves; i++){
		Wave& wave = waves[i];
		wave.time += FIXED_TIMESTEP;
		if (wave.decay >= 1.f)
		{
			// wave.sign = -1.f;
			wave.grow = false;
		}
		else if ((wave.decay <= 0.f && !wave.grow) || minimumX(wave) > STAGE_WIDTH)
		{
			wave = Wave();
			for(short j = i; j < sea.numWaves-1; ++j) {
				waves[j] = waves[j+1];
			}
			--sea.numWaves;
		}
		if (wave.decay <= 1 && wave.grow){
			wave.decay += FIXED_TIMESTEP * WAVE_POSITIVE_DECAY_MULTIPLIER;
		} else {
			wave.decay += FIXED_TIMESTEP * WAVE_NEGATIVE_DECAY_MULTIPLIER;
		}

		// Update velocity with timestep
		wave.velocity[0] += FIXED_TIMESTEP * 0.1f * wave.amplitude;
		wave.velocity += dragForce(wave.velocity, 1.225f, .4f);
		// Update position with velocity;
		wave.position += wave.velocity;
	}
}


inline float heightAtX(const Wave& wave, float x){
	// get the height of the wave at x
	// Cool gaussian
	return wave.amplitude * wave.decay * pow(E, -pow(WAVE_WIDTH_MULTIPLIER * (x - wave.position.x), 2));
}

float slopeAtX(const Wave & wave, float x)
{
	// derivative of height
	return heightAtX(wave, x) * (-2 * WAVE_WIDTH_MULTIPLIER * (x - wave.position.x) * WAVE_WIDTH_MULTIPLIER);
}

float minimumX(const Wave& wave) {
	return wave.position.x - 2.5f * (1 / WAVE_WIDTH_MULTIPLIER); // solve for heightAtX == 0
}

float maximumX(const Wave& wave) {
	return wave.position.x + 2.5f * (1 / WAVE_WIDTH_MULTIPLIER);
}

int createWave(Sea& sea, Vector2 position, float amplitude){
	if(sea.numWaves >= MAX_WAVES) {
		return -1;
	}
	short new_wave_idx = sea.numWaves;
	Wave& new_wave = *(sea.waves + new_wave_idx);
	new_wave = Wave();
	new_wave.position = position;
	new_wave.amplitude = amplitude;
	new_wave.active = true;
	new_wave.id = ++sea.id_src;
	return sea.numWaves++;
}

