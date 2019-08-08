#pragma once

#include "aabb.hpp"
#include "stage.hpp"
#include "util.hpp"


void resolveCollisions(Stage& stage);
void dispatchPotentialCollision(Stage& stage, const AABB& a, const AABB& b);

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
