#pragma once
#include <algorithm>
#include <vector>

#include "aabb.hpp"
#include "constants.hpp"
#include "maths.hpp"
#include "typedefs.hpp"
#include "wave.hpp"


struct Sea{
	// Wave waves[MAX_WAVES];
	// short numWaves = 0;
	std::vector<Wave> waves;
	float level = 0;
	uint8 id = 0;
	static uint8 id_src;
};

float heightAtX(const Sea& sea, float x); // return the y height of a sea at x
Vector2 velocityAtX(const Sea& sea, float x); // return the velocity of all the waves at this poisition
float slopeAtX(const Sea& sea, float x); // return the slope of the sea at x
void fixedUpdate(Sea& sea); // Update the sea for this frame of the simulation
void destroyWave(Sea& sea, Wave*);

