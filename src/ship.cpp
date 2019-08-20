#include "aabb.hpp"
#include "constants.hpp"
#include "entity.hpp"
#include "physics.hpp"
#include "ship.hpp"


void updateShip(Ship& ship){
	ship.velocity += GRAVITY * FIXED_TIMESTEP;
	// auto drag = dragForce(ship.velocity, 1.225f, ship.shape.radius * ROCK_RADIUS_MASS_RATIO);
}

