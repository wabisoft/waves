#pragma once
#include <vector>

#include <glm/vec2.hpp>

#include "aabb.hpp"
#include "prelude.hpp"
#include "shapes.hpp"
#include "util.hpp"
#include "win.hpp"

void updateBroadPhase(Stage& stage);
void resolveCollisions(Stage& stage);
void dispatchPotentialCollision(Stage& stage, const AABBPair& pair);

// Rocks Collide with things:
//  - Seas
//  - Platforms
//  - Ships
//  - Rocks

void collide(Rock& rock, Sea& sea);
void collide(Rock& rock, const Platform& platform); // platforms probably shouldn't change
void collide(Rock& rock, Ship& ship);
void collide(Rock& rock, Rock& other_rock);

// Ships collide with things:
//  - Seas
//  - Platforms
//  - Rocks
//	- Win
void collide(Ship& ship, Sea& sea);
void collide(Ship& ship, const Platform& platform); // a ship shouldn't alter a platform
void collide(Ship& ship, Rock& rock);
void collide(Ship& ship, Win& win);


struct Collision {
	bool collides = false;
	float penetration;
	glm::vec2 intersection;
	glm::vec2 normal;
	glm::vec2 surfaceStart = {0, 0};
	glm::vec2 surfaceEnd = {0, 0};
};

Collision collision(const glm::vec2 p1Pos, const glm::vec2 p2Pos, const wabi::Polygon& p1, const wabi::Polygon& p2);

// special clips
wabi::Polygon clip(const Entity&, const Sea&);
