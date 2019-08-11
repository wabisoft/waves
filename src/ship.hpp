#pragma once

#include "prelude.hpp"
#include "vector2.hpp"

struct Ship {
	Vector2 position = {0.f, 0.f};
	Vector2 velocity = {0.f, 0.f};
	float rotation = 0.f; // radians or degrees? 
	float width = 5.f;  // meters
	float height = 3.f; // meters
	int id = -1;
};

void fixedUpdate(Ship& ship);
