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
	if (stage.paused) {
		return;
	}
	resolveCollisions(stage);
	// TODO: check for collisions of all AABBs
	fixedUpdateRocks(stage);
	// for (int i = 0; i < stage.numRocks; i++){
	// 	fixedUpdate(stage.rocks[i]);
	// }
	fixedUpdate(stage.sea);
	fixedUpdate(stage.ship);
}

