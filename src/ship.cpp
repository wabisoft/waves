#include "aabb.hpp"
#include "constants.hpp"
#include "entity.hpp"
#include "physics.hpp"
#include "ship.hpp"


void updateShip(Ship& ship){
	ship.velocity += GRAVITY * FIXED_TIMESTEP;
	auto drag = dragForce(ship.velocity, 1.225f, mass(ship) * SHIP_AREA_MASS_RATIO);
	ship.velocity += drag * FIXED_TIMESTEP;
	ship.shape.position += ship.velocity;
	updateVertices(ship.shape);
}

