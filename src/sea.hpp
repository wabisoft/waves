#pragma once
#include <algorithm>
#include <vector>

#include "aabb.hpp"
#include "constants.hpp"
#include "maths.hpp"
#include "shapes.hpp"
#include "typedefs.hpp"
#include "wave.hpp"


struct Sea{
	// Wave waves[MAX_WAVES];
	// short numWaves = 0;
	wabi::Rectangle shape;
	std::vector<Wave> waves;
	// float level = 0;
	uint8 id = 0;
	static uint8 id_src;

	inline float heightAtX(float x) const; // return the y height of a sea at x
	inline Vector2 velocityAtX(float x) const; // return the velocity of all the waves at this poisition
	inline float slopeAtX(float x) const; // return the slope of the sea at x
};

typedef std::vector<Sea>::iterator SeaIt;

#include "sea.inl"

void updateSeas(Stage& stage);
uint8 createSea(Stage& stage, Vector2 position, float width, float height);
uint8 createSea(Stage& stage, wabi::Rectangle rectangle);
SeaIt deleteSea(Stage& stage, SeaIt seaIt);
SeaIt deleteSea(Stage& stage, uint8 seaId);
SeaIt findSea(Stage& stage, uint8 seaId);

