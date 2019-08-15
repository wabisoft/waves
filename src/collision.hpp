#pragma once

#include "aabb.hpp"
#include "stage.hpp"
#include "util.hpp"


struct AABBPair{
	AABB a;
	AABB b;
};

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
void collide(Ship& ship, const Sea& sea); // a ship probably shouldn't alter a sea
void collide(Ship& ship, const Platform& platform); // a ship shouldn't alter a platform either
void collide(Ship& ship, Rock& rock);

struct Collision {
	bool collides = false;
	Vector2 intersection;
	Vector2 normal;
	float penetration;
};

template <int N, int M>
Collision collision(Polygon<N>& poly1, Polygon<M>& poly2);
template <int N>
Collision collision(Circle& circle, Polygon<N>& polygon);

#include "collision.inl"
