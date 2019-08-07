#pragma once

#include "vector2.hpp"

struct Ship {
	Vector2 position = {0.f, 0.f};
	Vector2 velocity = {0.f, 0.f};
	float rotation;
	float width = 5.f;  // meters
	float height = 3.f; // meters
};

void fixedUpdate(Ship& ship);
