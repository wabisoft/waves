#pragma once
#include <algorithm>
#include <cstdint>

#include "aabb.hpp"
#include "constants.hpp"
#include "maths.hpp"
#include "wave.hpp"


struct Sea{
	Wave waves[MAX_WAVES];
	short numWaves = 0;
	float level = 0;
	uint8_t id = 0;
	static uint8_t id_src;
};

float heightAtX(const Sea& sea, float x); // return the y height of a sea at x
float slopeAtX(const Sea& sea, float x); // return the slope of the sea at x
void fixedUpdate(Sea& sea); // Update the sea for this frame of the simulation
void destroyWave(Sea& sea, Wave*);

