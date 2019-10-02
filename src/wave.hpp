#pragma once

#include <cstdint>
#include <vector>

#include "aabb.hpp"
#include "constants.hpp"
#include "prelude.hpp"
#include "maths.hpp"
#include "typedefs.hpp"
#include "util.hpp"


struct Wave {
	Vector2 position = {0.f, 0.f};
	Vector2 velocity = {0.f, 0.f};
	float amplitude = 0.f;
	float decay = 0.f;
	float time = 0.f;
	int8 direction = 1;
	int8 sign = 1;
	bool active = false;
	bool grow = true;
	uint8 id = 0;

	inline float heightAtX(float x) const;
	inline Vector2 velocityAtX(float x) const;
	inline float slopeAtX(float x) const; // derivative of height
	inline float minimumX() const;
	inline float maximumX() const;
};

#include "wave.inl"

typedef std::vector<Wave>::iterator WaveIt;

void updateWaves(Stage& stage, Sea& sea);
uint8 createWave(Sea& sea, Vector2 position, float amplitude, int direction, int sign);
WaveIt deleteWave(Sea& sea, WaveIt waveIt);
WaveIt deleteWave(Sea& sea, uint8 waveId);
WaveIt findWave(Sea& sea, uint8 waveId);
WaveIt findWaveAtPosition(Sea& sea, Vector2 position);
