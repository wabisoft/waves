#pragma once

#include "aabb.hpp"
#include "prelude.hpp"
// #include "stage.hpp"
#include "util.hpp"


void resolveCollisions(Stage& stage);
void dispatchPotentialCollision(Stage& stage, const AABBPair& pair);

// Rocks Collide with things:
//  - Sea
//  - Platforms
//  - Ships
//  - Rocks

void collide(Rock& rock, Sea& sea);
void collide(Rock& rock, const Platform& platform); // platforms probably shouldn't change
void collide(Rock& rock, Ship& ship);
void collide(Rock& rock, Rock& other_rock);

// Ships collide with things:
//  - Sea
//  - Platforms
//  - Rocks
void collide(Ship& ship, Sea& sea);
void collide(Ship& ship, const Platform& platform); // a ship shouldn't alter a platform
void collide(Ship& ship, Rock& rock);

struct Collision {
	bool collides = false;
	Vector2 intersection;
	Vector2 normal;
	float penetration;
	Vector2 surfaceStart = VECTOR2_ZERO;
	Vector2 surfaceEnd = VECTOR2_ZERO;
};

template <int N, int M>
Collision collision(const Polygon<N>& poly1, const Polygon<M>& poly2);
template <int N>
Collision collision(const Circle& circle, const Polygon<N>& polygon);
Collision collision(const Circle& c1, const Circle& c2);

#include "collision.inl"
