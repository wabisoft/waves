#pragma once

#include <cstdint>
#include "aabb.hpp"
#include "constants.hpp"
#include "prelude.hpp"
#include "maths.hpp"
#include "util.hpp"


struct Wave {
	inline Wave() {}
	Vector2 position = {0.f, 0.f};
	Vector2 velocity = {0.f, 0.f};
	float amplitude = 0.f;
	float decay = 0.f;
	float time = 0.f;
	short direction = 1;
	short sign = 1;
	bool active = false;
	bool grow = true;
	uint8_t id = 0;
};

float heightAtX(const Wave& wave, float x);
Vector2 velocityAtX(const Wave& wave, float x);
float slopeAtX(const Wave & wave, float x); // derivative of height
float minimumX(const Wave& wave);
float maximumX(const Wave& wave);

void updateWaves(Stage& stage);
int createWave(Sea& sea, Vector2 position, float amplitude, short direction, short sign);

int findWaveAtX(const Sea& sea, float x);
Wave& findWave(Sea& sea, uint8_t wave_id);
