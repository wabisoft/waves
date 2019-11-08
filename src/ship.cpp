#include "aabb.hpp"
#include "settings.hpp"
#include "entity.hpp"
#include "physics.hpp"
#include "ship.hpp"
#include "stage.hpp"

using namespace wabi;
using namespace glm;

inline void updateFallingShip(Ship& ship) {
	ship.velocity += GRAVITY * FIXED_TIMESTEP;
	// auto drag = dragForce(ship.velocity, 1.225f, mass(ship) * SHIP_AREA_MASS_RATIO);
	auto dims = ship.shape.model[1] - ship.shape.model[3];
	auto drag = dragForce(ship.velocity, AIR_DENSITY, std::max(dims.x, dims.y));
	// auto drag = dragForce(ship.velocity, 1.225f, mass(ship));
	ship.velocity += drag * FIXED_TIMESTEP;
	ship.position += ship.velocity;
	ship.omega += ship.alpha * FIXED_TIMESTEP;
	ship.shape.rotation += ship.omega * FIXED_TIMESTEP;
	ship.alpha = 0;
	// ship.omega *= 0.98;
}

inline void updateStandingShip(Stage& stage, float deltaTime) {
	Ship& ship = stage.ship;
	assert(ship.state.type == Ship::State::STANDING);

	ship.velocity *= 0.75;
	ship.velocity += GRAVITY * FIXED_TIMESTEP;
	ship.position += ship.velocity;

	ship.omega += ship.alpha * FIXED_TIMESTEP;
	ship.shape.rotation += ship.omega * FIXED_TIMESTEP;
	ship.alpha = 0;
	// ship.omega *= 0.98;

	vec2 futurePos = ship.position + ship.velocity;
	vec2 & start = ship.state.standing.surfaceStart;
	vec2 & end = ship.state.standing.surfaceEnd;
	vec2 start2Rock = futurePos - start;
	vec2 start2End = end - start;
	vec2 normalizedStart2End = normalize(start2End);
	float proj = dot(start2Rock, normalizedStart2End);
	vec2 anchor = start +  normalizedStart2End * proj;
	vec2 anchorRelPos = anchor - ship.position;
	float sqMagAnchorRelPos = dot(anchorRelPos, anchorRelPos);
	float shipHeight = (ship.shape.model[0] - ship.shape.model[2]).y;
	float sqShipHalfHeight = (shipHeight/2) * (shipHeight / 2);
	bool bound = bounded(start, end, anchor);
	bool anchorOnSurface = sqMagAnchorRelPos <= sqShipHalfHeight;
	bool staysInContact = bound && anchorOnSurface;
	if (!staysInContact) {
		ship.state = {Ship::State::FALLING, {}};
	}

	if(ship.inWin) {
		stage.win.timeInArea += deltaTime;
	}
	// bool inWinRegion = true;
	// for(int i = 0; i < ship.shape.size; ++i) {
	// 	inWinRegion &= pointInside(ship.shape.vertices[i], stage.win.shape);
	// 	if (!inWinRegion) {break;}
	// }
	// if (inWinRegion) {
	// 	stage.win.timeInArea += deltaTime;
	// }
}

inline void updateSurfingShip(Stage& stage, Ship& ship) {
	// TODO: Keep the ship on the crest of the wave
	Sea& sea = *findSea(stage, ship.state.surfing.seaId);
	Wave & wave = *findWave(sea, ship.state.surfing.waveId);
	if (wave.direction != ship.state.surfing.waveDirection) {
		ship.state = {Ship::State::FALLING, {}}; // If the wave has just reflected then stop surfing
	} else {
		ship.velocity.x = wave.velocity.x * wave.direction; // otherwise just ride the wave man
	}
	updateFallingShip(ship); // do all the same things you do for a falling ship
	// ship.position.x = wave.position.x;
}

void updateShip(Stage& stage, float deltaTime){
	Ship& ship = stage.ship;
	switch (ship.state.type) {
		case Ship::State::FALLING:  updateFallingShip(ship); break;
		case Ship::State::STANDING: updateStandingShip(stage, deltaTime); break;
		case Ship::State::SURFING: updateSurfingShip(stage, ship); break;
	}
	update(ship.shape, ship.position);
	vec2 & shipPos = ship.position;
	// if (shipPos.x > STAGE_WIDTH || shipPos.x < 0 || shipPos.y < 0) {
	if (shipPos.x > STAGE_WIDTH || shipPos.x < 0) {
		stage.state.type = StageState::FINISHED;
		stage.state.finished.win = false;
		// stage.failed = true;
	}
}


u8 createShip(Stage& stage, vec2 position, float width, float height) {
	stage.ship = Ship(makeRectangle(width, height), position, ++stage.id_src);
	createAABB(stage, AABB(stage.ship));
	return stage.ship.id;
}

u8 createShip(Stage& stage, glm::vec2 position, Polygon shape) {
	stage.ship = Ship(shape, position, ++stage.id_src);
	createAABB(stage, AABB(stage.ship));
	return stage.ship.id;
}


