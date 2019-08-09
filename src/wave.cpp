#include "constants.hpp"
#include "sea.hpp"
#include "wave.hpp"

void fixedUpdateWaves(Sea& sea){
	//Update the wave for this time step
	Wave* waves = sea.waves;
	for(size_t i = 0; i < sea.numWaves; i++){
		waves[i].time += FIXED_TIMESTEP * 2;
		if (waves[i].decay >= 1.f)
		{
			waves[i].sign = -0.25f;
		}
		else if (waves[i].decay <= 0.f && waves[i].sign <= 0)
		{
			waves[i].active = false;
			waves[i].sign = 0;
			waves[i].decay = 0;
		}
		waves[i].decay = waves[i].decay + waves[i].sign * FIXED_TIMESTEP;
		// Update velocity with timestep
		waves[i].velocity[0] += FIXED_TIMESTEP;
		// Update position with velocity;
		waves[i].position += waves[i].velocity;
	}
}


inline float heightAtX(const Wave& wave, float x){
	// get the height of the wave at x
	// Cool gaussian
	return wave.amplitude * wave.decay * pow(E, -pow(wave.width * (x - wave.position.x), 2));
}

float slopeAtX(const Wave & wave, float x)
{
	// derivative of height
	return heightAtX(wave, x) * (-2 * wave.width * (x - wave.position.x) * wave.width);
}

float minimumX(const Wave& wave) {
	return wave.position.x - 2.5f * (1 / wave.width); // solve for heightAtX == 0
}

float maximumX(const Wave& wave) {
	return wave.position.x + 2.5f * (1 / wave.width);
}

int createWave(Sea& sea, Vector2 position, float amplitude){
	if(sea.numWaves >= MAX_WAVES) {
		return -1;
	}
	size_t new_wave_idx = sea.numWaves;
	Wave& new_wave = *(sea.waves + new_wave_idx);
	new_wave = Wave();
	new_wave.position = position;
	new_wave.amplitude = amplitude;
	new_wave.active = true;
	new_wave.id = ++sea.id_src;	
	return sea.numWaves++;
}

