#include <iostream>
#include <limits>
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
	assert(stage.id_src < (uint8)(-1)); // 255
	switch(stage.state.type) {
		case StageState::PAUSED:
			stage.state.paused.time += deltaTime;
			return; break;
		// NOTE: the absence of a break continues the switch, viz. default will still trigger
		case StageState::RUNNING: break;
			stage.state.running.time += deltaTime;
		case StageState::FINISHED: break;
	}
	if(stage.rocks.size() < 1) {
		createRock(stage, stage.rockSpawn, ROCK_MAX_RADIUS - 1, stage.rockType);
	}
	resolveCollisions(stage);
	updateRocks(stage, deltaTime);
	updateSeas(stage);
	updateShip(stage, deltaTime);
	if(stage.win.timeInArea >= stage.win.timeToWin) {
		stage.state.type = StageState::FINISHED;
		stage.state.finished.win = true;
	}
}

Entity makeSelectionAtPosition(Stage& stage, Vector2 position) {
	// For now we only select rocks so let's find our rock
	RockIt rockIt = findRockAtPosition(stage, position);
	if (rockIt != stage.rocks.end()) {
		stage.selection.entity = { rockIt->id, ROCK };
		stage.selection.active = true;
		stage.selection.entityPosition = rockIt->shape.position;
	}
	return stage.selection.entity;
}

Entity findEntityAtPosition(Stage& stage, Vector2 position) {
	// a list of pairs of {Axis of overlap, entity}
	struct Candidate{
		Axis axis = NO_AXIS;
		Entity entity;
	};
	std::vector<Candidate> candidates;
	for(int a = 0; a < 2; ++a) {
		std::vector<uint8> axisOrder = stage.xAxisOrder;
		Axis axis = X_AXIS;
		if(a>0) {
			axisOrder = stage.yAxisOrder;
			axis = Y_AXIS;
		}
		for(int i = 0; i < axisOrder.size(); ++i) {
			AABB aabb = *findAABB(stage, axisOrder[i]);
			if(aabb.lower[a] <= position[a] && aabb.upper[a] >= position[a]) {
				auto pred = [&aabb](Candidate c) -> bool {
					return c.entity.id == aabb.id;
				};
				auto search = std::find_if(candidates.begin(), candidates.end(), pred);
				if(search != candidates.end()) {
					search->axis = (Axis)(search->axis | axis);
				} else if (a<1) { // if we are on the second axis and the candidate doesn't exist then we can skip
					candidates.push_back({axis, Entity{aabb.id, aabb.type}});
				}
			} else if (aabb.lower[a] >= position[a]) {
				// since the axis list is sorted from lowest to highest then
				// we can say that everything after this is not overlaping
				break;
			}
		}
	}
	struct Chosen {
		float squaredDistance;
		Entity entity;
	};
	Chosen chosen = {std::numeric_limits<float>::infinity(), {0, NONE}};
	for(Candidate & candidate : candidates) {
		switch(candidate.entity.type) {
			case NONE:
				assert(false); // this should never happen
				break;
			case SEA:
				{
					Sea sea = *findSea(stage, candidate.entity.id);
					float squaredDistance = squaredMagnitude(sea.shape.position - position);
					if(chosen.squaredDistance > squaredDistance) {
						chosen = {squaredDistance, {sea.id, SEA}};
					}
				}
				break;
			case PLATFORM:
				{
					Platform platform = *findPlatform(stage, candidate.entity.id);
					float squaredDistance = squaredMagnitude(platform.shape.position - position);
					if(chosen.squaredDistance > squaredDistance) {
						chosen = {squaredDistance, {platform.id, PLATFORM}};
					}
				}
				break;
			case ROCK:
				{
					Rock rock = *findRock(stage, candidate.entity.id);
					float squaredDistance = squaredMagnitude(rock.shape.position - position);
					if(chosen.squaredDistance > squaredDistance) {
						chosen = {squaredDistance, {rock.id, ROCK}};
					}
				}
				break;
			case SHIP:
				{
					Ship ship = stage.ship;
					float squaredDistance = squaredMagnitude(ship.shape.position - position);
					if(chosen.squaredDistance > squaredDistance) {
						chosen = {squaredDistance, {ship.id, SHIP}};
					}
				}
				break;
		}
	}
	return chosen.entity;
}

void clearSelection(Stage& stage) {
	stage.selection = {}; // clears the selection
}

inline bool validateAndSetPullPosition(Stage& stage,  Vector2 position) {
	assert(stage.selection.active);
	Rock& rock = *findRock(stage, stage.selection.entity.id);
	Vector2 pull = rock.shape.position - position;
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
			}// else if (stage.selection.entity.type == ROCK) {
			// 	Rock& rock = findRock(stage, stage.selection.entity.id);
			// 	if (!rock.sized){
			// 		stage.selection.state = Selection::RESIZE;
			// 	} else {
			// 		stage.selection.state= Selection::PULL;
			// 	}
			// }
			stage.selection.state= Selection::PULL;
			break;
		// case Selection::RESIZE:
		// 	makeSelectionAtPosition(stage, position);
		// 	break;
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
	case Selection::PULL: { // if we're throwing
		assert(stage.selection.active); // I think we should always have a selection in this phase
		validateAndSetPullPosition(stage, position);
		Rock& rock = *findRock(stage, stage.selection.entity.id);
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
	Rock& rock = *findRock(stage, stage.selection.entity.id);
	Vector2 pull = rock.shape.position - stage.selection.pullPosition;
	float pullLength = std::abs(magnitude(pull));
	float throwMag = (pullLength / STAGE_MAX_PULL_LENGTH) * ROCK_MAX_SPEED;
	return (pull/pullLength) * throwMag;

}

std::vector<Vector2> pullParabola(Stage& stage) {
	assert(stage.selection.active && stage.selection.state == Selection::PULL);
	std::vector<Vector2> parabola;
	Rock& rock = *findRock(stage, stage.selection.entity.id);
	Vector2 f = rock.velocity + getPullForce(stage);
	Vector2 wouldBePosition = rock.shape.position;
	const int stepsize = 10;
	const float step = stepsize*FIXED_TIMESTEP; // look at the projectile would be position every 10 updates
	const float t = 1.5; // (seconds)
	const int size = (int)(t/step)+1;
	parabola.reserve(size);
	parabola.push_back(wouldBePosition);
	for (int i = 1; i < size; ++i){
		f += GRAVITY * step;
		f += dragForce(f, 1.225f, mass(rock)) * step;
		if (squaredMagnitude(f) > SQUARED_TERMINAL_VELOCITY) {
			f = normalized(f) * TERMINAL_VELOCITY;
		}
		wouldBePosition += f * stepsize;
		parabola.push_back(wouldBePosition);
		if(outOfBounds(wouldBePosition)) { break; }
	}
	return parabola;
}

