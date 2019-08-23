#pragma once

#include "prelude.hpp"
#include "maths.hpp"

struct Ship {
	Rectangle shape;
	Vector2 velocity;
	uint8_t id = 0;
};

void updateShip(Ship& ship);

inline float mass(const Ship& ship) {
	return area(ship.shape) * SHIP_AREA_MASS_RATIO;
}
