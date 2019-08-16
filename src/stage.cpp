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
	// if (stage.numRocks < 1) {
	// 	createRock(stage, stage.rockSpawn, 3.f);
	// }
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

