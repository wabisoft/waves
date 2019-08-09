#pragma once

#include <cmath>

#include "aabb.hpp"
#include "constants.hpp"
#include "wave.hpp"


struct Sea{
	inline Sea() { }
	inline Sea(float l) : level(l) { }
	Wave waves[MAX_WAVES];
	Wave inactiveWaves[MAX_WAVES];
	size_t numWaves = 0;
	float level = 0;
	unsigned int id = 0;
};

inline AABB aabb(const Sea& sea) {
	float maxHeight = sea.level;
	for(size_t i = 0; i < sea.numWaves; ++i) {
		maxHeight = std::max(maxHeight, heightAtX(sea.waves[i], sea.waves[i].position.x));
	}
	Vector2 lower = {0.f, 0.f};
	Vector2 upper = {STAGE_WIDTH, maxHeight};
	return AABB(lower, upper, SEA, sea.id); 
}

float heightAtX(const Sea& sea, float x); // return the y height of a sea at x
float slopeAtX(const Sea& sea, float x); // return the slope of the sea at x
void fixedUpdate(Sea& sea); // Update the sea for this frame of the simulation
void createWave(Sea& sea, Vector2 position, float amplitude);
void destroyWave(Sea& sea, Wave*);

