#pragma once

#include <cmath>

#include "AABB.hpp"
#include "constants.hpp"
#include "prelude.hpp"
#include "util.hpp"
#include "vector2.hpp"



struct Wave {
	inline Wave() {}
	inline Wave(const Vector2& position) : position(position) {}
	inline Wave(const Vector2& position, float amplitude) : position(position), amplitude(amplitude) {}
	explicit inline Wave(const Wave& wave) :
		position(wave.position),
		velocity(wave.velocity),
		amplitude(wave.amplitude),
		decay(wave.decay),
		time(wave.time),
		width(wave.width) {
	}

	Vector2 position = {0.f, 0.f};
	Vector2 velocity = {0.f, 0.f};
	float amplitude = 0.f;
	float decay = 0.f;
	float sign = 1.f;
	float time = 0.f;
	float width = 0.1f;
};

inline float heightAtX(const Wave& wave, float x){
	// get the height of the wave at x
	// Cool gaussian
	return wave.amplitude * wave.decay * pow(E, -pow(wave.width * (x - wave.position.x), 2));
}

float slopeAtX(const Wave & wave, float x); // derivative of height

inline bool active(const Wave& wave) {
	return wave.decay <= 0.f && wave.sign <= 0; // sign is negative (wave getting smaller) and decay is back down to nothing (wave is donezo)
}

inline float minimumX(const Wave& wave) {
	return wave.position.x - 2.5f * (1 / wave.width); // solve for heightAtX == 0
}

inline float maximumX(const Wave& wave) {
	return wave.position.x + 2.5f * (1 / wave.width);
}

// I don't think we need a wave bounds, everything should just collide with the sea
// inline AABB aabb(const Wave& wave){
// 	return AABB(Vector2(minimumX(wave), wave.position.y), Vector2(maximumX(wave), heightAtX(wave, wave.position.x)), WAVE, wave.id);
// }

void fixedUpdate(Wave& wave);
