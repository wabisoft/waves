#pragma once

#include <cmath>

#include "aabb.hpp"
#include "constants.hpp"
#include "prelude.hpp"
#include "util.hpp"
#include "vector2.hpp"



struct Wave {
	inline Wave() {}
	// inline Wave(const Vector2& position) : position(position) {}
	// inline Wave(const Vector2& position, float amplitude) : position(position), amplitude(amplitude) {}
	// explicit inline Wave(const Wave& wave) :
	// 	position(wave.position),
	// 	velocity(wave.velocity),
	// 	amplitude(wave.amplitude),
	// 	decay(wave.decay),
	// 	time(wave.time),
	// 	width(wave.width) {
	// }

	Vector2 position = {0.f, 0.f};
	Vector2 velocity = {0.f, 0.f};
	float amplitude = 0.f;
	float decay = 0.f;
	float sign = 5.f;
	float time = 0.f;
	float width = 0.1f;
	bool active = false;
	unsigned id;
};

float heightAtX(const Wave& wave, float x);
float slopeAtX(const Wave & wave, float x); // derivative of height
float minimumX(const Wave& wave);
float maximumX(const Wave& wave);

void fixedUpdateWaves(Sea& sea);
int createWave(Sea& sea, Vector2 position, float amplitude);
