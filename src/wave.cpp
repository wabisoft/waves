#include "constants.hpp"
#include "physics.hpp"
#include "sea.hpp"
#include "ship.hpp"
#include "stage.hpp"
#include "wave.hpp"

void updateWaves(Stage& stage){
	//Update the wave for this time step
	Sea& sea = stage.sea;
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
			// delete this wave
			if (stage.ship.state.type == ShipState::SURFING && stage.ship.state.surfing.wave_id == wave.id) {
				stage.ship.state = {ShipState::FALLING, {}};
				stage.ship.velocity = wave.velocity;
			}
			wave = Wave();
			for(short j = i; j < sea.numWaves-1; ++j) {
				waves[j] = waves[j+1];
				waves[j+1] = Wave();
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


float heightAtX(const Wave& wave, float x){
	// get the height of the wave at x
	// Cool gaussian
	return wave.amplitude * wave.decay * pow(E, -pow(WAVE_WIDTH_MULTIPLIER * (x - wave.position.x), 2));
}

Vector2 velocityAtX(const Wave& wave, float x) {
	float distFromMid = x - wave.position.x;
	if(distFromMid < 0) {
		return VECTOR2_ZERO; // Don't do anything on the back of a wave
	} else if (distFromMid == 0) {
		return wave.velocity; // if you're at the middle of the wave get the full velocity
	} else { // else get the velocity as a ratio of your distance the center
		float max = maximumX(wave);
		if (max < x) { return VECTOR2_ZERO; }
		float min = minimumX(wave);
		float halfLength = (max - min)/2.f;
		return (halfLength - distFromMid) * wave.velocity;
	}
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


int findWaveAtX(const Sea& sea, float x) { // returns the index of the closest wave
	float min = INF;
	int idx = -1;
	for (int i = 0 ; i < sea.numWaves; ++i) {
		float diff = std::abs(x - sea.waves[i].position.x);
		if ( diff < min) {
			min = diff; idx = i;
		}
	}
	return idx;
}

Wave& findWave(Sea& sea, uint8_t wave_id) {
	// waves should be in the waves array in order of their id
	int wave_idx = binary_find_where(wave_id, sea.waves, sea.numWaves, [](const Wave& wave){return wave.id;});
	assert(wave_idx > -1); // see binary find where
	return sea.waves[wave_idx];
}
