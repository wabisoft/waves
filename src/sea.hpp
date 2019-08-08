#pragma once

#include <list>
#include "constants.hpp"
#include "wave.hpp"


struct Sea{
	inline Sea() { }
	inline Sea(float l) : level(l) { }
	// std::list<Wave*> waves = { };
	Wave waves[MAX_WAVES];
	Wave inactiveWaves[MAX_WAVES];
	float level = 0;
};


float heightAtX(const Sea& sea, float x); // return the y height of a sea at x
float slopeAtX(const Sea& sea, float x); // return the slope of the sea at x
void fixedUpdate(Sea& sea); // Update the sea for this frame of the simulation
void createWave(Sea& sea, Vector2 position, float amplitude);
void destroyWave(Sea& sea, Wave*);

