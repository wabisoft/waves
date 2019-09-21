#include <iostream>

#include "aabb.hpp"
#include "collision.hpp"
#include "constants.hpp"
#include "entity.hpp"
#include "physics.hpp"
#include "rock.hpp"
#include "stage.hpp"
#include "util.hpp"

inline void updateFallingRock(Stage& stage, Rock& rock, int rock_idx, float deltaTime) {
	assert(rock.state.type == RockState::FALLING);
	rock.velocity += GRAVITY * deltaTime;
	auto drag = dragForce(rock.velocity, 1.225f, rock.shape.radius * ROCK_RADIUS_MASS_RATIO);
	rock.velocity += drag * deltaTime;
	if(squaredMagnitude(rock.velocity) > SQUARED_TERMINAL_VELOCITY) {
		rock.velocity = normalized(rock.velocity) * TERMINAL_VELOCITY;
	}
	rock.shape.position += rock.velocity;
}

inline void updateStandingRock(Stage& stage, Rock& rock, int rock_idx, float deltaTime) {
	// TODO: this function does a lot of useless caching for debugging purposes,
	// clean that up eventually
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
}

inline void updateFloatingRock(Stage& stage, Rock& rock, int rock_idx, float deltaTime) {
	assert(rock.state.type == RockState::FLOATING);
	rock.state.floating.timeFloating += deltaTime;
	if(rock.state.floating.timeFloating > ROCK_MAX_FLOAT_TIME) {
		createWave(stage.sea, rock.shape.position, 2* rock.shape.radius * rock.shape.radius * PI, (short)sign(rock.velocity.x));
		if (stage.selection.entity.id == rock.id) {
			// if this rock is selected we need to clear it
			clearSelection(stage);
		}
		deleteRockByIdx(stage, rock_idx);
	}
	rock.velocity += GRAVITY * deltaTime;
	auto drag = dragForce(rock.velocity, 1.225f, rock.shape.radius * ROCK_RADIUS_MASS_RATIO);
	rock.velocity += drag * deltaTime;
	rock.shape.position += rock.velocity;

}

void updateRocks(Stage& stage, float deltaTime) {
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
			case RockState::FALLING: updateFallingRock(stage, rocks[i], i,  deltaTime); break;
			case RockState::STANDING: updateStandingRock(stage, rocks[i], i, deltaTime); break;
			case RockState::FLOATING: updateFloatingRock(stage, rocks[i], i, deltaTime); break;
		}
	}
}

uint8_t createRock(Stage& stage, Vector2 position, float radius, RockType type){
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
	new_rock.type = type;
	stage.numRocks++;
	// stage.usedRocks++;
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

int findRockAtPosition(const Stage& stage, Vector2 position) {
	for (int i = 0; i < stage.numRocks; ++i) {
		float dist = magnitude(position - stage.rocks[i].shape.position);
		if (dist <= stage.rocks[i].shape.radius) {
			return i;
		}
	}
	return -1;
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


