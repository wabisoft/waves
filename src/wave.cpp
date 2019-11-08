#include <algorithm>

#include "settings.hpp"
#include "maths.hpp"
#include "physics.hpp"
#include "sea.hpp"
#include "ship.hpp"
#include "stage.hpp"
#include "wave.hpp"

using namespace glm;

void updateWaves(Stage& stage, Sea& sea){
	//Update the wave for this time step
	for(WaveIt waveIt = sea.waves.begin(); waveIt != sea.waves.end(); ++waveIt) {
		Wave& wave = *waveIt;
		wave.time += FIXED_TIMESTEP;
		if (wave.decay >= 1.f)
		{
			// wave.sign = -1.f;
			wave.grow = false;
		}
		else if ((wave.decay <= 0.f && !wave.grow) || wave.minimumX() > STAGE_WIDTH)
		{
			// delete this wave
			if (stage.ship.state.type == Ship::State::SURFING && stage.ship.state.surfing.waveId == wave.id) {
				stage.ship.state = {Ship::State::FALLING, {}};
				stage.ship.velocity = wave.velocity;
			}
			waveIt = deleteWave(sea, waveIt);
			if(waveIt == sea.waves.end()) break; // don't advance past the end
		}
		if(wave.position.x >= upperBound(sea.shape).x || wave.position.x <= lowerBound(sea.shape).x) {
			wave.direction = -wave.direction;
		}
		if (wave.decay <= 1 && wave.grow){
			wave.decay += FIXED_TIMESTEP * WAVE_POSITIVE_DECAY_MULTIPLIER;
		} else {
			wave.decay += FIXED_TIMESTEP * WAVE_NEGATIVE_DECAY_MULTIPLIER;
		}
		// Update velocity with timestep
		wave.velocity[0] += FIXED_TIMESTEP * 0.05f * wave.amplitude;
		wave.velocity += dragForce(wave.velocity, 1.225f, .4f);
		// Update position with velocity;
		wave.position += (float)wave.direction * wave.velocity;
	}
}


u8 createWave(Sea& sea, vec2 position, float amplitude, int direction, int sign){
	Wave new_wave;
	new_wave.position = position;
	new_wave.amplitude = amplitude;
	new_wave.active = true;
	if (direction != -1 && direction != 1) { direction = 1;}
	new_wave.direction = direction;
	assert(sign == -1 || sign == 1);
	new_wave.sign = sign;
	new_wave.id = ++sea.id_src;
	sea.waves.push_back(new_wave);
	return new_wave.id;
}

WaveIt deleteWave(Sea& sea, WaveIt waveIt) {
	return sea.waves.erase(waveIt);
}

WaveIt deleteWave(Sea& sea, u8 waveId) {
	return deleteWave(sea, findWave(sea, waveId));
}

WaveIt findWave(Sea& sea, u8 waveId) {
	// waves should be in the waves array in order of their id
	WaveIt waveIt = std::lower_bound(sea.waves.begin(), sea.waves.end(), waveId, [](const Wave& w, u8 id) -> bool { return w.id < id; });
	assert(waveIt != sea.waves.end());
	return waveIt;
}

WaveIt findWaveAtPosition(Sea& sea, vec2 position) { // returns the index of the closest wave
	float min = INF;
	WaveIt retIt = sea.waves.end();
	for(WaveIt waveIt = sea.waves.begin(); waveIt != sea.waves.end(); ++waveIt) {
		float diff = std::abs(position.x - waveIt->position.x);
		if ( diff < min) {
			min = diff; retIt = waveIt;
		}
	}
	return retIt;
}

