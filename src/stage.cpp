#include "platform.hpp"
#include "rock.hpp"
#include "sea.hpp"
#include "ship.hpp"
#include "stage.hpp"
#include "wave.hpp"



void fixedUpdate(Stage& stage, float deltaTime){
	if (stage.paused) {
		return;
	}
	// TODO: check for collisions of all AABBs
	fixedUpdate(stage.rocks, stage.numRocks);
	// for (int i = 0; i < stage.numRocks; i++){
	// 	fixedUpdate(stage.rocks[i]);
	// }
	fixedUpdate(stage.sea);
	fixedUpdate(stage.ship);
}

void resolveCollisions(Stage& stage) {

}
