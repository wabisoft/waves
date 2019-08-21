#include <iostream>

#include "aabb.hpp"
#include "collision.hpp"
#include "constants.hpp"
#include "entity.hpp"
#include "physics.hpp"
#include "rock.hpp"
#include "stage.hpp"
#include "util.hpp"

inline void updateFallingRock(Stage& stage, Rock& rock, int rock_idx) {
	assert(rock.state.type == RockState::FALLING);
	rock.velocity += GRAVITY * FIXED_TIMESTEP;
	auto drag = dragForce(rock.velocity, 1.525f, rock.shape.radius * ROCK_RADIUS_MASS_RATIO);
	rock.velocity += drag * FIXED_TIMESTEP;
	if(squaredMagnitude(rock.velocity) > SQUARED_TERMINAL_VELOCITY) {
		rock.velocity = normalized(rock.velocity) * TERMINAL_VELOCITY;
	}
	rock.shape.position += rock.velocity;
}

inline void updateStandingRock(Stage& stage, Rock& rock, int rock_idx) {
	assert(rock.state.type == RockState::STANDING);
	Vector2 futurePos = rock.shape.position + rock.velocity;
	Vector2& start = rock.state.standing.surfaceStart;
	Vector2& end = rock.state.standing.surfaceEnd;
	Vector2 start2Rock = futurePos - start;
	Vector2 start2End = end - start;
	float proj = dot(start2Rock, start2End);
	Vector2 anchor = start + normalized(start2End) * proj;
	bool staysInContact = sideSign(start, end, anchor) == 0 && bounded(start, end, anchor);
	if (!staysInContact) {
		rock.state = {RockState::FALLING, {}};
		return updateFallingRock(stage, rock, rock_idx);
	} else {
	}
}

void updateRocks(Stage& stage) {
	Rock* rocks = stage.rocks;
	for (int i = 0; i < stage.numRocks; ++i) {
		clamp(rocks[i].velocity, ROCK_MAX_SPEED); // clamp the rock speed
		if (rocks[i].shape.position.x < 0 || rocks[i].shape.position.x > STAGE_WIDTH || rocks[i].shape.position.y < 0 || rocks[i].active == false) {
			// remove our aabb from the stage aabb array
			if (stage.selection.entity.id == rocks[i].id) {
				// if this rock is selected we need to clear it
				clearSelection(stage);
			}
			deleteRockByIdx(stage, i);
		}
		switch (rocks[i].state.type) {
			case RockState::FALLING: updateFallingRock(stage, rocks[i], i); break;
			case RockState::STANDING: updateStandingRock(stage, rocks[i], i); break;
		}
	}
}

uint8_t createRock(Stage& stage, Vector2 position, float radius){
	// NOTE (!!!): The implementation of the routine must keep rocks in order of id so that the array can be
	// binary searched in findRock
	if (stage.numRocks >= MAX_ROCKS){
		// Ideally there are a fixed number of rocks and we shouldn't need any error handling here
		// but just incase
		return -1;
	}
	int new_rock_idx = stage.numRocks;
	Rock& new_rock = *(stage.rocks + new_rock_idx);
	new_rock = Rock();
	new_rock.active = true;
	new_rock.id = ++stage.id_src;
	new_rock.shape.position = position;
	new_rock.shape.radius = radius;
	stage.numRocks++;
	createAABB(stage, AABB(new_rock));
	return new_rock.id;
}

inline int deleteRockByIdx(Stage& stage, int rock_idx){
	deleteAABBById(stage, stage.rocks[rock_idx].id);
	stage.rocks[rock_idx] = Rock();
	for (int j = rock_idx; j < stage.numRocks-1; j++){
			stage.rocks[j] = stage.rocks[j+1];
			stage.rocks[j+1] = Rock();
	}
	return --stage.numRocks;
}

inline int deleteRockById(Stage& stage, uint8_t rock_id){
	Rock * rocks = stage.rocks;
	for(int i = 0; i < stage.numRocks; i++){
		if (rocks[i].id == rock_id){
			return deleteRockByIdx(stage, i);
		}
	}
	return -1;
}

Rock& findRock(Stage& stage, uint8_t rock_id) {
	int rock_idx = binary_find_where(rock_id, stage.rocks, stage.numRocks, [](const Rock& rock){return rock.id;});
	assert(rock_idx > -1); // see binary find where
	return stage.rocks[rock_idx];
}

Entity findRockAtPosition(Stage& stage, Vector2 position) {
	for (int i = 0; i < stage.numRocks; ++i) {
		float dist = magnitude(position - stage.rocks[i].shape.position);
		if (dist <= stage.rocks[i].shape.radius) {
			return {stage.rocks[i].id, ROCK};
		}
	}
	return {};
}

void resizeRock(Stage& stage, int rock_id, Vector2 position){
	Rock& rock = findRock(stage, rock_id);
	Vector2 relPos = position - rock.shape.position;
	float squaredSize = squaredMagnitude(relPos);
	if(squaredSize > ROCK_MIN_RADIUS_SQUARED && squaredSize < ROCK_MAX_RADIUS_SQUARED) {
		rock.shape.radius = std::sqrt(squaredSize);
	}
	rock.sized = true;
}


