#include <iostream>
#include "constants.hpp"
#include "gravity.hpp"
#include "rock.hpp"
#include "stage.hpp"


void fixedUpdate(Rock * rocks, size_t& numRocks) {
	for (int i = 0; i < numRocks; ++i) {
		if (rocks[i].position.x < 0 || rocks[i].position.x > STAGE_WIDTH || rocks[i].position.y < 0 || rocks[i].position.y > STAGE_HEIGHT) {
			rocks[i].active = false;
			for (int j = i; j < numRocks-1; j++){
				rocks[j] = rocks[j+1];
			}
			--numRocks;
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


