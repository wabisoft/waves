#include <iostream>
#include "collision.hpp"
#include "constants.hpp"
#include "platform.hpp"
#include "printing.hpp"
#include "rock.hpp"
#include "sea.hpp"
#include "ship.hpp"
#include "stage.hpp"
#include "wave.hpp"
#include "util.hpp"



void update(Stage& stage, float deltaTime){
	if (stage.paused) {
		return;
	}

	if (stage.numRocks < 1) {
		createRock(stage, stage.rockSpawn, ROCK_MAX_RADIUS - 1);
	}
	if (stage.numAABBS > MAX_AABBS)
		std::cout << "what the actual fuck" << std::endl;
	resolveCollisions(stage);
	updateRocks(stage);
	updateWaves(stage.sea);
	updateShip(stage.ship);
}


Entity makeSelectionAtPosition(Stage& stage, Vector2 position) {
	// For now we only select rocks so let's find our rock
	Entity entity = findRockAtPosition(stage, position);
	if (entity.id && entity.type != NONE) { // id defaults 0 and is always > 0 if it refers to an entity
		stage.selection.entity = entity;
		stage.selection.active = true;
	}
	return entity;
}

void clearSelection(Stage& stage) {
	stage.selection = {};
	stage.phase = SELECT;
}

inline bool validateAndSetPullPosition(Stage& stage,  Vector2 position) {
	assert(stage.selection.active);
	Rock& rock = findRock(stage, stage.selection.entity.id);
	float squaredPullLength = squaredMagnitude(rock.shape.position - position);
	if(squaredPullLength < STAGE_MAX_PULL_LENGTH_SQUARED) {
		stage.pullPosition = position;
		return true;
	} else {
		Vector2 relPos = normalized(position - rock.shape.position);
		stage.pullPosition = rock.shape.position + relPos * STAGE_MAX_PULL_LENGTH;
		return true;
	}
}

void processStartInput(Stage& stage, Vector2 position) {
	if (stage.phase == SELECT) {
		makeSelectionAtPosition(stage, position);
		if (! stage.selection.active) {
			return;
		} else if (stage.selection.entity.type == ROCK) {
			Rock& rock = findRock(stage, stage.selection.entity.id);
			if (!rock.sized){
				stage.phase = RESIZE;
			} else {
				stage.phase = PULL;
			}
		}
	}
	if(stage.phase == RESIZE) {
		makeSelectionAtPosition(stage, position);
	} else if (stage.phase == PULL) {
		if(validateAndSetPullPosition(stage, position)){
			stage.phase = PULL;
		}
	}
}

void processContinuingInput(Stage& stage, Vector2 position) {
	if(stage.phase == SELECT){
		// Such empty...
	} else if (stage.phase == RESIZE) {
		if (stage.selection.active && stage.selection.entity.type == ROCK) {
			resizeRock(stage, stage.selection.entity.id, position);
		} else {
			return;
		}
	} else if (stage.phase == PULL) {
		validateAndSetPullPosition(stage, position);
	}
}

void processEndInput(Stage& stage, Vector2 position) {
	if (stage.phase == SELECT) {
		// such empty ...
	} else if (stage.phase == RESIZE) { // if we're resizing
		if (stage.selection.active) { // and we have a selection
			resizeRock(stage, stage.selection.entity.id, position);
			stage.phase = SELECT; // move on to throwing
			return;
		} else {
			stage.selection = Selection(); // otherwise safety clear the selection and move on
			return;
		}
	} else if (stage.phase == PULL){ // if we're throwing
		assert(stage.selection.active); // I think we should always have a selection in this phase
		validateAndSetPullPosition(stage, position);
		// TODO: Throw the rock
		Rock& rock = findRock(stage, stage.selection.entity.id);
		Vector2 pull = rock.shape.position - stage.pullPosition;
		float pullLength = std::abs(magnitude(pull));
		float throwMag = (pullLength / STAGE_MAX_PULL_LENGTH) * ROCK_MAX_SPEED * 0.025f; // YAY magic!
		// float throwMag = (pullLength / STAGE_MAX_PULL_LENGTH) * ROCK_MAX_SPEED;
		rock.shape.position += 0.01f * pull; // If you dont so this then to collision get all weird and bad things happen
		Vector2 force = (pull/pullLength) * throwMag;
		rock.velocity += force;
		// rock.state = FALLING;
		stage.selection = {};
		stage.pullPosition = VECTOR2_ZERO;
		stage.phase = SELECT;
	}
}
