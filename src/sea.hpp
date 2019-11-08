#pragma once
#include <algorithm>
#include <vector>

#include <glm/vec2.hpp>

#include "aabb.hpp"
#include "settings.hpp"
#include "maths.hpp"
#include "shapes.hpp"
#include "wave.hpp"


struct Sea : Entity {
	// ctors & dtors
	Sea() {}
	Sea(wabi::Polygon shape, glm::vec2 position, u8 id) : Entity(Entity::SEA, shape, position, id) { }

	// methods
	float heightAtX(float x) const; // return the y height of a sea at x
	glm::vec2 velocityAtX(float x) const; // return the velocity of all the waves at this poisition
	float slopeAtX(float x) const; // return the slope of the sea at x

	// members
	std::vector<Wave> waves;
	// statics
	static u8 id_src;
};

typedef std::vector<Sea>::iterator SeaIt;

void updateSeas(Stage& stage);
u8 createSea(Stage& stage, glm::vec2 position, float width, float height);
u8 createSea(Stage& stage, glm::vec2 position, wabi::Polygon&);
SeaIt deleteSea(Stage& stage, SeaIt seaIt);
SeaIt deleteSea(Stage& stage, u8 seaId);
SeaIt findSea(Stage& stage, u8 seaId);

