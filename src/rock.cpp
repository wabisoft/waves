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

using namespace glm;
using namespace wabi;

inline RockIt updateFallingRock(Stage& stage, RockIt rockIt, float deltaTime) {
	Rock& rock = *rockIt;
	assert(rock.state.type == Rock::State::FALLING);
	rock.velocity += GRAVITY * deltaTime;
	auto drag = dragForce(rock.velocity, 1.225f, rock.radius * ROCK_RADIUS_MASS_RATIO);
	rock.velocity += drag * deltaTime;
	if(dot(rock.velocity, rock.velocity) > SQUARED_TERMINAL_VELOCITY) {
		rock.velocity = normalize(rock.velocity) * TERMINAL_VELOCITY;
	}
	rock.position += rock.velocity;
	return rockIt;
}

inline RockIt updateStandingRock(Stage& stage, RockIt rockIt, float deltaTime) {
	// TODO: this function does a lot of useless caching for debugging purposes,
	// clean that up eventually
	Rock& rock = *rockIt;
	assert(rock.state.type == Rock::State::STANDING);
	// Do the actual update
	rock.velocity *= 0.75; // shitty friction
	rock.velocity += GRAVITY * deltaTime;
	rock.position += rock.velocity;

	// Calculate if we should not be standing anymore
	vec2 futurePos = rock.position + rock.velocity;
	vec2 & start = rock.state.standing.surfaceStart;
	vec2 & end = rock.state.standing.surfaceEnd;
	vec2 start2Rock = futurePos - start;
	vec2 start2End = end - start;
	vec2 normalizedStart2End = normalize(start2End);
	float proj = dot(start2Rock, normalizedStart2End);
	vec2 anchor = start +  normalizedStart2End * proj;
	vec2 anchorRelPos = anchor - rock.position;
	float sqMagAnchorRelPos = dot(anchorRelPos, anchorRelPos);
	float sqrockRadius = rock.radius * rock.radius;
	bool bound = bounded(start, end, anchor);
	bool anchorOnSurface = sqMagAnchorRelPos <= sqrockRadius;
	bool staysInContact = bound && anchorOnSurface;
	if (!staysInContact) {
		rock.state = {Rock::State::FALLING, {}};
	}
	return rockIt;
}

inline RockIt updateFloatingRock(Stage& stage, RockIt rockIt, float deltaTime) {
	Rock& rock = *rockIt;
	assert(rock.state.type == Rock::State::FLOATING);
	rock.state.floating.time += deltaTime;
	if(rock.state.floating.time > ROCK_MAX_FLOAT_TIME) {
		Sea& sea = *findSea(stage, rock.state.floating.seaId);
		createWave(sea, rock.position, 2* rock.radius * rock.radius * PI, (short)sign(rock.velocity.x), 1);
		if (stage.selection.entity && stage.selection.entity->id == rock.id) {
			// if this rock is selected we need to clear it
			clearSelection(stage);
		}
		return deleteRock(stage, rockIt);
	}
	rock.velocity += GRAVITY * deltaTime;
	auto drag = dragForce(rock.velocity, 1.225f, rock.radius * ROCK_RADIUS_MASS_RATIO);
	rock.velocity += drag * deltaTime;
	rock.position += rock.velocity;
	return rockIt;
}

inline RockIt updateSinkingRock(Stage& stage, RockIt rockIt, float deltaTime) {
	Rock& rock = *rockIt;
	assert(rock.state.type == Rock::State::SINKING);
	rock.velocity += GRAVITY * deltaTime;
	auto drag = dragForce(rock.velocity, 1000.f, rock.radius * ROCK_RADIUS_MASS_RATIO);
	rock.velocity += drag * deltaTime;
	if(dot(rock.velocity, rock.velocity) > SQUARED_TERMINAL_VELOCITY) {
		rock.velocity = normalize(rock.velocity) * TERMINAL_VELOCITY;
	}
	rock.position += rock.velocity;
	rock.state.sinking.time += deltaTime;
	if (rock.state.sinking.time > 0.25f) {
		return deleteRock(stage, rockIt);
	}
	return rockIt;
}

void updateRocks(Stage& stage, float deltaTime) {
	for (RockIt rockIt = stage.rocks.begin(); rockIt != stage.rocks.end(); ++rockIt) {
		Rock& rock = *rockIt;
		clamp(rock.velocity, ROCK_MAX_SPEED); // clamp the rock speed
		if (rock.position.x < 0 || rock.position.x > STAGE_WIDTH || rock.position.y < 0) {
			// remove our aabb from the stage aabb array
			rockIt = deleteRock(stage, rockIt);
		} else {
			switch (rock.state.type) {
				case Rock::State::FALLING:  rockIt = updateFallingRock(stage, rockIt, deltaTime); break;
				case Rock::State::STANDING: rockIt = updateStandingRock(stage, rockIt, deltaTime); break;
				case Rock::State::FLOATING: rockIt = updateFloatingRock(stage, rockIt, deltaTime); break;
				case Rock::State::SINKING:  rockIt = updateSinkingRock(stage, rockIt, deltaTime); break;
			}
		}
		if (rockIt == stage.rocks.end()) break; // because our vector can change sizes while we're iterating
		update(rockIt->shape, rockIt->position);
	}
}

uint8 createRock(Stage& stage, vec2 position, float radius, Rock::Kind kind){
	// NOTE (!!!): The implementation of the routine must keep rocks in order of id so that the array can be
	// binary searched in findRock
	Rock new_rock(makeCircle(radius), position, ++stage.id_src, radius, kind);
	stage.rocks.push_back(new_rock);
	createAABB(stage, AABB(new_rock));
	return new_rock.id;
}

uint8 createRock(Stage& stage, const Polygon& polygon, vec2 position, float radius, Rock::Kind kind){
	// NOTE (!!!): The implementation of the routine must keep rocks in order of id so that the array can be
	// binary searched in findRock
	Rock new_rock(polygon, position, ++stage.id_src, radius, kind);
	stage.rocks.push_back(new_rock);
	createAABB(stage, AABB(new_rock));
	return new_rock.id;
}


RockIt deleteRock(Stage& stage, RockIt rockIt) {
	if (stage.selection.entity && stage.selection.entity->id == rockIt->id) {
		// if this rock is selected we need to clear it
		clearSelection(stage);
	}
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

RockIt findRockAtPosition(Stage& stage, vec2 position) {
	for(RockIt rockIt = stage.rocks.begin(); rockIt != stage.rocks.end(); ++rockIt){
		float dist = length(position - rockIt->position);
		if (dist <= rockIt->radius) {
			return rockIt;
		}
	}
	return stage.rocks.end();
}

// void resizeRock(Stage& stage, int rock_id, vec2 position){
// 	Rock& rock = *findRock(stage, rock_id);
// 	vec2 relPos = position - rock.position;
// 	float squaredSize = dot(relPos, relPos);
// 	if(squaredSize > ROCK_MIN_RADIUS_SQUARED && squaredSize < ROCK_MAX_RADIUS_SQUARED) {
// 		rock.shape.radius = std::sqrt(squaredSize);
// 	}
// 	rock.sized = true;
// }


