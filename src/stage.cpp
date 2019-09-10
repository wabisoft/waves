#include <iostream>
#include "collision.hpp"
#include "constants.hpp"
#include "maths.hpp"
#include "physics.hpp"
#include "platform.hpp"
#include "printing.hpp"
#include "rock.hpp"
#include "sea.hpp"
#include "ship.hpp"
#include "stage.hpp"
#include "wave.hpp"
#include "util.hpp"



void update(Stage& stage, float deltaTime){
	switch(stage.state.type) {
		case StageState::PAUSED:
			stage.state.paused.time += deltaTime;
			return; break;

		// NOTE: the absence of a break continues the switch, viz. default will still trigger
		case StageState::RUNNING:
			stage.state.running.time += deltaTime;
		case StageState::FINISHED:
		default: // keep running the simulation even if the game is over
			if(stage.numRocks < 1) {
				createRock(stage, stage.rockSpawn, ROCK_MAX_RADIUS - 1);
			}
			resolveCollisions(stage);
			updateRocks(stage);
			updateWaves(stage);
			updateShip(stage, deltaTime);
			if(stage.win.timeInArea >= stage.win.timeToWin) {
				stage.state.type = StageState::FINISHED;
				stage.state.finished.win = true;
			}
			break;
	}
}


Entity makeSelectionAtPosition(Stage& stage, Vector2 position) {
	// For now we only select rocks so let's find our rock
	int rockIdx = findRockAtPosition(stage, position);
	if (rockIdx >= 0 && rockIdx < stage.numRocks) {
		Rock& rock = stage.rocks[rockIdx];
		stage.selection.entity = {rock.id, ROCK};
		stage.selection.active = true;
		stage.selection.entityPosition = rock.shape.position;
	}
	return stage.selection.entity;
}

void clearSelection(Stage& stage) {
	stage.selection = {}; // clears the selection
}

inline bool validateAndSetPullPosition(Stage& stage,  Vector2 position) {
	assert(stage.selection.active);
	Rock& rock = findRock(stage, stage.selection.entity.id);
	Vector2 pull = rock.shape.position - position;
	// if(rock.state.type == RockState::STANDING) {
	// 	Vector2 surfaceVector = rock.state.standing.surfaceEnd - rock.state.standing.surfaceStart;
	// 	float pullAngle = cross(surfaceVector, pull) / (magnitude(surfaceVector) * magnitude(pull));
	// 	if (pullAngle <= 0.15) { // a x b = |a||b|sin(theta) and is sin > 0 for 0 < theta < PI and sin < 0 for PI < theta < 2*PI
	// 		return false;
	// 	}
	// }
	float squaredPullLength = squaredMagnitude(pull);
	if(squaredPullLength < STAGE_MAX_PULL_LENGTH_SQUARED) {
		stage.selection.pullPosition = position;
		return true;
	} else {
		Vector2 relPos = normalized(position - rock.shape.position);
		stage.selection.pullPosition = rock.shape.position + relPos * STAGE_MAX_PULL_LENGTH;
		return true;
	}
}

void processStartInput(Stage& stage, Vector2 position) {

	switch(stage.selection.state) {
		case Selection::SELECT:
			makeSelectionAtPosition(stage, position);
			if (! stage.selection.active) {
				return;
			} else if (stage.selection.entity.type == ROCK) {
				Rock& rock = findRock(stage, stage.selection.entity.id);
				if (!rock.sized){
					stage.selection.state = Selection::RESIZE;
				} else {
					stage.selection.state= Selection::PULL;
				}
			}
			break;
		case Selection::RESIZE:
			makeSelectionAtPosition(stage, position);
			break;
		case Selection::PULL:
			if(validateAndSetPullPosition(stage, position)){
				stage.selection.state = Selection::PULL;
			}
			break;
	}
}

void processContinuingInput(Stage& stage, Vector2 position) {
	switch(stage.selection.state) {
		case Selection::SELECT:
			// Such empty...
 			break;
		case Selection::RESIZE:
			if (stage.selection.active && stage.selection.entity.type == ROCK) {
				resizeRock(stage, stage.selection.entity.id, position);
			} else {
				return;
			}
		break;
		case Selection::PULL:
			validateAndSetPullPosition(stage, position);
			break;
	}
}

void processEndInput(Stage& stage, Vector2 position) {
	switch(stage.selection.state) {
		case Selection::SELECT:
			// such empty ...
		break;
	case Selection::RESIZE: // if we're resizing
		if (stage.selection.active) { // and we have a selection
			resizeRock(stage, stage.selection.entity.id, position);
			stage.selection.state = Selection::SELECT; // move on to throwing
			return;
		} else {
			stage.selection = Selection(); // otherwise safety clear the selection and move on
			return;
		}
		break;
	case Selection::PULL: { // if we're throwing
		assert(stage.selection.active); // I think we should always have a selection in this phase
		validateAndSetPullPosition(stage, position);
		Rock& rock = findRock(stage, stage.selection.entity.id);
		Vector2 pull = rock.shape.position - stage.selection.pullPosition;
		float pullLength = std::abs(magnitude(pull));
		float throwMag = (pullLength / STAGE_MAX_PULL_LENGTH) * ROCK_MAX_SPEED;
		rock.shape.position += 0.01f * pull; // If you dont so this then to collision get all weird and bad things happen
		Vector2 force = (pull/pullLength) * throwMag;
		rock.velocity += force;
		rock.state = {RockState::FALLING, {}};
		stage.selection = Selection{};
		stage.selection.pullPosition = VECTOR2_ZERO;
		clearSelection(stage);
		}
		break;
	}
}

Vector2 getPullForce(Stage& stage) {
	assert(stage.selection.active && stage.selection.state == Selection::PULL);
	Rock& rock = findRock(stage, stage.selection.entity.id);
	Vector2 pull = rock.shape.position - stage.selection.pullPosition;
	float pullLength = std::abs(magnitude(pull));
	float throwMag = (pullLength / STAGE_MAX_PULL_LENGTH) * ROCK_MAX_SPEED;
	// rock.shape.position += 0.01f * pull; // If you dont so this then to collision get all weird and bad things happen
	return (pull/pullLength) * throwMag;

}

std::vector<Vector2> pullParabola(Stage& stage) {
	assert(stage.selection.active && stage.selection.state == Selection::PULL);
	std::vector<Vector2> parabola;
	Rock& rock = findRock(stage, stage.selection.entity.id);
	Vector2 f = rock.velocity + getPullForce(stage);
	Vector2 wouldBePosition = rock.shape.position;
	const int stepsize = 5;
	const float step = stepsize*FIXED_TIMESTEP; // look at the projectile would be position every 10 updates
	const float t = 2; // (seconds)
	const int size = (int)(t/step)+1;
	parabola.reserve(size);
	parabola.push_back(wouldBePosition);
	for (int i = 1; i < size; ++i){
		f += GRAVITY * step;
		wouldBePosition += f * stepsize;
		parabola.push_back(wouldBePosition);
		if(outOfBounds(wouldBePosition)) { break; }
	}
	return parabola;
}
