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



void fixedUpdate(Stage& stage, float deltaTime){
	if (stage.numRocks < 1) {
		createRock(stage, stage.rockSpawn, ROCK_START_RADIUS);
	}
	if (stage.paused) {
		return;
	}
	if (stage.numAABBS > MAX_AABBS)
		std::cout << "what the actual fuck" << std::endl;
	resolveCollisions(stage);
	fixedUpdateRocks(stage);
	fixedUpdateWaves(stage.sea);
	fixedUpdate(stage.ship);
}


void resizeRock(Stage& stage, int rock_id, Vector2 position){
	Rock& rock = findRock(stage, rock_id);
	Vector2 relPos = position - rock.shape.position;
	float size = magnitude(relPos);
	if(size > ROCK_MIN_RADIUS && size < ROCK_MAX_RADIUS) {
		rock.shape.radius = size;
	}
}

Rock& findRockAtPosition(Stage& stage, Vector2 position) {
	for (int i = 0; i < stage.numRocks; ++i) {
		if (magnitude(position - stage.rocks[i].shape.position) <= stage.rocks[i].shape.radius) {
			return stage.rocks[i];
		}
	}
	// TODO: Maybe we need to look for other things
}
