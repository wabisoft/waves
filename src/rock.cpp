#include <iostream>

#include "collision.hpp"
#include "constants.hpp"
#include "gravity.hpp"
#include "rock.hpp"
#include "stage.hpp"


void fixedUpdateRocks(Stage& stage) {
	Rock* rocks = stage.rocks;
	for (size_t i = 0; i < stage.numRocks; ++i) {
		if (rocks[i].position.x < 0 || rocks[i].position.x > STAGE_WIDTH || rocks[i].position.y < 0 || rocks[i].position.y > STAGE_HEIGHT) {
			rocks[i].active = false;
			for (size_t j = i; j < stage.numRocks-1; j++){
				rocks[j] = rocks[j+1];
			}
			--stage.numRocks;
		}
		if(rocks[i].position.y - rocks[i].radius < heightAtX(stage.sea, rocks[i].position.x)) { 
			// collision detection with sea is very easy on a per rock basis, and only complicates the collision detection broad phase if done there.
			// so instead we do it here.
			collide(rocks[i], stage.sea);
		}
		
		rocks[i].velocity += GRAVITY * FIXED_TIMESTEP;
		rocks[i].position += rocks[i].velocity;
	}
}

int createRock(Stage& stage, Vector2 position, float radius){
	if (stage.numRocks >= MAX_ROCKS){
		// Ideally there are a fixed number of rocks and we shouldn't need any error handling here
		// but just incase
		return -1;
	}
	size_t new_rock_idx = stage.numRocks;
	Rock* new_rock = stage.rocks + new_rock_idx;
	new_rock->active = true;
	new_rock->id = ++stage.id_src;
	new_rock->position = position;
	new_rock->velocity = { 0.f, 0.f };
	new_rock->radius = radius;
	stage.numRocks++;
	// insertion_sort(stage.rocks, stage.numRocks, [](Rock a, Rock b) { return aabb(a).lower.x < aabb(b).lower.x; });
	return new_rock->id;
}

int deleteRock(Stage& stage, int rock_id){
	int rock_idx = -1;
	Rock * rocks = stage.rocks;
	for(size_t i = 0; i < stage.numRocks; i++){
		if (rocks[i].id == rock_id){
			rock_idx = i;
		} else if(rock_idx > 0){
			rocks[i-1] = rocks[i];
		}
	}		
	return --stage.numRocks;
}

int deleteInactiveRocks(Stage& stage){
	return -1;
}


