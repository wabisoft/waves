#pragma once

#include "prelude.hpp"
#include "maths.hpp"

struct Ship {
	Rectangle shape;
	Vector2 velocity;
	uint8_t id = 0;
};

void updateShip(Ship& ship);
