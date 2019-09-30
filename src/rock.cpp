#include <iostream>

#include "typedefs.hpp"
#include "aabb.hpp"
#include "collision.hpp"
#include "constants.hpp"
#include "entity.hpp"
#include "physics.hpp"
#include "rock.hpp"
#include "stage.hpp"
#include "util.hpp"

inline RockIt updateFallingRock(Stage& stage, RockIt rockIt, float deltaTime) {
	Rock& rock = *rockIt;
	assert(rock.state.type == RockState::FALLING);
	rock.velocity += GRAVITY * deltaTime;
	auto drag = dragForce(rock.velocity, 1.225f, rock.shape.radius * ROCK_RADIUS_MASS_RATIO);
	rock.velocity += drag * deltaTime;
	if(squaredMagnitude(rock.velocity) > SQUARED_TERMINAL_VELOCITY) {
		rock.velocity = normalized(rock.velocity) * TERMINAL_VELOCITY;
	}
	rock.shape.position += rock.velocity;
	return rockIt;
}

inline RockIt updateStandingRock(Stage& stage, RockIt rockIt, float deltaTime) {
	// TODO: this function does a lot of useless caching for debugging purposes,
	// clean that up eventually
	Rock& rock = *rockIt;
	assert(rock.state.type == RockState::STANDING);
	// Do the actual update
	rock.velocity *= 0.75; // shitty friction
	rock.velocity += GRAVITY * deltaTime;
	rock.shape.position += rock.velocity;

	// Calculate if we should not be standing anymore
	Vector2 futurePos = rock.shape.position + rock.velocity;
	Vector2& start = rock.state.standing.surfaceStart;
	Vector2& end = rock.state.standing.surfaceEnd;
	Vector2 start2Rock = futurePos - start;
	Vector2 start2End = end - start;
	Vector2 normalizedStart2End = normalized(start2End);
	float proj = dot(start2Rock, normalizedStart2End);
	Vector2 anchor = start +  normalizedStart2End * proj;
	Vector2 anchorRelPos = anchor - rock.shape.position;
	float sqMagAnchorRelPos = squaredMagnitude(anchorRelPos);
	float sqrockRadius = rock.shape.radius * rock.shape.radius;
	bool bound = bounded(start, end, anchor);
	bool anchorOnSurface = sqMagAnchorRelPos <= sqrockRadius;
	bool staysInContact = bound && anchorOnSurface;
	if (!staysInContact) {
		rock.state = {RockState::FALLING, {}};
	}
	return rockIt;
}

inline RockIt updateFloatingRock(Stage& stage, RockIt rockIt, float deltaTime) {
	Rock& rock = *rockIt;
	assert(rock.state.type == RockState::FLOATING);
	rock.state.floating.timeFloating += deltaTime;
	if(rock.state.floating.timeFloating > ROCK_MAX_FLOAT_TIME) {
		createWave(stage.sea, rock.shape.position, 2* rock.shape.radius * rock.shape.radius * PI, (short)sign(rock.velocity.x), 1);
		if (stage.selection.entity.id == rock.id) {
			// if this rock is selected we need to clear it
			clearSelection(stage);
		}
		return deleteRock(stage, rockIt);
	}
	rock.velocity += GRAVITY * deltaTime;
	auto drag = dragForce(rock.velocity, 1.225f, rock.shape.radius * ROCK_RADIUS_MASS_RATIO);
	rock.velocity += drag * deltaTime;
	rock.shape.position += rock.velocity;
	return rockIt;
}

void updateRocks(Stage& stage, float deltaTime) {
	for (RockIt rockIt = stage.rocks.begin(); rockIt != stage.rocks.end(); ++rockIt) {
		Rock& rock = *rockIt;
		clamp(rock.velocity, ROCK_MAX_SPEED); // clamp the rock speed
		if (rock.shape.position.x < 0 || rock.shape.position.x > STAGE_WIDTH || rock.shape.position.y < 0 || rock.active == false) {
			// remove our aabb from the stage aabb array
			if (stage.selection.entity.id == rock.id) {
				// if this rock is selected we need to clear it
				clearSelection(stage);
			}
			rockIt = deleteRock(stage, rockIt);
		} else {
			switch (rock.state.type) {
				case RockState::FALLING:  rockIt = updateFallingRock(stage, rockIt, deltaTime); break;
				case RockState::STANDING: rockIt = updateStandingRock(stage, rockIt, deltaTime); break;
				case RockState::FLOATING: rockIt = updateFloatingRock(stage, rockIt, deltaTime); break;
			}
		}
		if (rockIt == stage.rocks.end()) break; // because our vector can change sizes while we're iterating
	}
}

uint8 createRock(Stage& stage, Vector2 position, float radius, RockType type){
	// NOTE (!!!): The implementation of the routine must keep rocks in order of id so that the array can be
	// binary searched in findRock
	Rock new_rock;
	new_rock.active = true;
	new_rock.id = ++stage.id_src;
	new_rock.shape.position = position;
	new_rock.shape.radius = radius;
	new_rock.type = type;
	stage.rocks.push_back(new_rock);
	createAABB(stage, AABB(new_rock));
	return new_rock.id;
}

RockIt deleteRock(Stage& stage, RockIt rockIt) {
	deleteAABB(stage, rockIt->id);
	return stage.rocks.erase(rockIt);
}

RockIt deleteRock(Stage& stage, uint8 rockId) {
	return deleteRock(stage, findRock(stage, rockId));
}

RockIt findRock(Stage& stage, uint8 rockId) {
	RockIt rockIt = std::lower_bound(stage.rocks.begin(), stage.rocks.end(), rockId, [](const Rock& r, uint8 id) -> bool { return r.id < id; });
	assert(rockIt != stage.rocks.end()); 
	return rockIt;
}

RockIt findRockAtPosition(Stage& stage, Vector2 position) {
	for(RockIt rockIt = stage.rocks.begin(); rockIt != stage.rocks.end(); ++rockIt){ 
		float dist = magnitude(position - rockIt->shape.position);
		if (dist <= rockIt->shape.radius) {
			return rockIt;
		}
	}
	return stage.rocks.end();
}

void resizeRock(Stage& stage, int rock_id, Vector2 position){
	Rock& rock = *findRock(stage, rock_id);
	Vector2 relPos = position - rock.shape.position;
	float squaredSize = squaredMagnitude(relPos);
	if(squaredSize > ROCK_MIN_RADIUS_SQUARED && squaredSize < ROCK_MAX_RADIUS_SQUARED) {
		rock.shape.radius = std::sqrt(squaredSize);
	}
	rock.sized = true;
}


