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

inline void updateStandingShip(Stage& stage, float deltaTime) {
	Ship& ship = stage.ship;
	assert(ship.state.type == ShipState::STANDING);
	ship.velocity *= 0.75;
	ship.velocity += GRAVITY * FIXED_TIMESTEP;
	ship.shape.position += ship.velocity;

	Vector2 futurePos = ship.shape.position + ship.velocity;
	Vector2& start = ship.state.standing.surfaceStart;
	Vector2& end = ship.state.standing.surfaceEnd;
	Vector2 start2Rock = futurePos - start;
	Vector2 start2End = end - start;
	Vector2 normalizedStart2End = normalized(start2End);
	float proj = dot(start2Rock, normalizedStart2End);
	Vector2 anchor = start +  normalizedStart2End * proj;
	Vector2 anchorRelPos = anchor - ship.shape.position;
	float sqMagAnchorRelPos = squaredMagnitude(anchorRelPos);
	float sqShipHalfHeight = (ship.shape.height/2) * (ship.shape.height / 2);
	bool bound = bounded(start, end, anchor);
	bool anchorOnSurface = sqMagAnchorRelPos <= sqShipHalfHeight;
	bool staysInContact = bound && anchorOnSurface;
	if (!staysInContact) {
		ship.state = {ShipState::FALLING, {}};
	}
	bool inWinRegion = true;
	for(int i = 0; i < ship.shape.size; ++i) {
		inWinRegion &= pointInsidePolygon(ship.shape.vertices[i], stage.win.region);
		if (!inWinRegion) {break;}
	}
	if (inWinRegion) {
		stage.win.timeInArea += deltaTime;
	}

}

inline void updateSurfingShip(Stage& stage, Ship& ship) {
	// TODO: Keep the ship on the crest of the wave
	Wave & wave = findWave(stage.sea, ship.state.surfing.wave_id);
	ship.velocity.x = wave.velocity.x;
	updateFallingShip(ship); // do all the same things you do for a falling ship
	// ship.shape.position.x = wave.position.x;
}

void updateShip(Stage& stage, float deltaTime){
	Ship& ship = stage.ship;
	switch (ship.state.type) {
		case ShipState::FALLING:  updateFallingShip(ship); break;
		case ShipState::STANDING: updateStandingShip(stage, deltaTime); break;
		case ShipState::SURFING: updateSurfingShip(stage, ship); break;
	}
	updateVertices(ship.shape);
	Vector2& shipPos = ship.shape.position;
	if (shipPos.x > STAGE_WIDTH || shipPos.x < 0 || shipPos.y < 0) {
		stage.state.type = StageState::FINISHED;
		stage.state.finished.win = false;
		// stage.failed = true;
	}
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
