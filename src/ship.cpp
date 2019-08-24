#include "aabb.hpp"
#include "constants.hpp"
#include "entity.hpp"
#include "physics.hpp"
#include "ship.hpp"
#include "stage.hpp"

inline void updateFallingShip(Ship& ship) {
	ship.velocity += GRAVITY * FIXED_TIMESTEP;
	auto drag = dragForce(ship.velocity, 1.225f, mass(ship) * SHIP_AREA_MASS_RATIO);
	ship.velocity += drag * FIXED_TIMESTEP;
	ship.shape.position += ship.velocity;
}

inline void updateStandingShip(Ship& ship) {
	// TODO: Something very similar to what we did in updateStandingRock
}

inline void updateSurfinShip(Ship& ship) {
	// TODO: Keep the ship on the crest of the wave
}

void updateShip(Ship& ship){
	switch (ship.state.type) {
		case ShipState::FALLING:  updateFallingShip(ship); break;
		case ShipState::STANDING: break;
		case ShipState::SURFIN: break;
	}
	updateVertices(ship.shape);
}


uint8_t createShip(Stage& stage, Vector2 position, float width, float height) {
	if (stage.ship.active){
		return -1; // we don't want to make more than one ship probably.
	}
	Ship& ship = stage.ship;
	ship.active = true;
	ship.id = ++stage.id_src;
	ship.shape = makeRectangle(position, width, height);
	ship.velocity = VECTOR2_ZERO;
	createAABB(stage, AABB(ship));
	return ship.id;
}
