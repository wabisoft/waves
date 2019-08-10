#pragma once
#include <algorithm>
#include <cstdint>

#include "aabb.hpp"
#include "constants.hpp"
#include "maths.hpp"
#include "wave.hpp"


struct Sea{
	inline Sea() { }
	inline Sea(float l) : level(l) { }
	Wave waves[MAX_WAVES];
	short numWaves = 0;
	float level = 0;
	uint8_t id = 0;
	static uint8_t id_src;
};

inline AABB aabb(const Sea& sea) {
	float maxHeight = sea.level;
	for(short i = 0; i < sea.numWaves; ++i) {
		maxHeight = std::max(maxHeight, heightAtX(sea.waves[i], sea.waves[i].position.x));
	}
	Vector2 lower = {0.f, 0.f};
	Vector2 upper = {STAGE_WIDTH, maxHeight};
	AABB aabb(lower, upper, SEA, sea.id); 
	aabb.entity = (void*)&sea;
	return aabb;
}

float heightAtX(const Sea& sea, float x); // return the y height of a sea at x
float slopeAtX(const Sea& sea, float x); // return the slope of the sea at x
void fixedUpdate(Sea& sea); // Update the sea for this frame of the simulation
void destroyWave(Sea& sea, Wave*);

