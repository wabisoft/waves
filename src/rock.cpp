#include <iostream>

#include "aabb.hpp"
#include "collision.hpp"
#include "constants.hpp"
#include "gravity.hpp"
#include "rock.hpp"
#include "stage.hpp"
#include "util.hpp"


void fixedUpdateRocks(Stage& stage) {
	Rock* rocks = stage.rocks;
	for (short i = 0; i < stage.numRocks; ++i) {
		if (rocks[i].position.x < 0 || rocks[i].position.x > STAGE_WIDTH || rocks[i].position.y < 0 || rocks[i].position.y > STAGE_HEIGHT || rocks[i].active == false) {
			// remove our aabb from the stage aabb array
			deleteRockByIdx(stage, i);
		}
		rocks[i].velocity += GRAVITY * FIXED_TIMESTEP;
		rocks[i].position += rocks[i].velocity;
	}
}

int createRock(Stage& stage, Vector2 position, float radius){
	// NOTE (!!!): The implementation of the routine must keep rocks in order of id so that the array can be
	// binary searched in findRock
	if (stage.numRocks >= MAX_ROCKS){
		// Ideally there are a fixed number of rocks and we shouldn't need any error handling here
		// but just incase
		return -1;
	}
	short new_rock_idx = stage.numRocks;
	Rock& new_rock = *(stage.rocks + new_rock_idx);
	new_rock = Rock();
	new_rock.active = true;
	new_rock.id = ++stage.id_src;
	new_rock.position = position;
	new_rock.radius = radius;
	stage.numRocks++;
	createAABB(stage, AABB(new_rock));
	return new_rock.id;
}

inline int deleteRockByIdx(Stage& stage, int rock_idx){
	deleteAABBById(stage, stage.rocks[rock_idx].id);
	stage.rocks[rock_idx] = Rock();
	for (short j = rock_idx; j < stage.numRocks-1; j++){
			stage.rocks[j] = stage.rocks[j+1];
			stage.rocks[j+1] = Rock();
	}
	return --stage.numRocks;
}

inline int deleteRockById(Stage& stage, uint8_t rock_id){
	Rock * rocks = stage.rocks;
	for(short i = 0; i < stage.numRocks; i++){
		if (rocks[i].id == rock_id){
			return deleteRockByIdx(stage, i);
		}
	}
	return -1;
}

Rock& findRock(Stage& stage, uint8_t rock_id) {
	size_t rock_idx = binary_find_where(rock_id, stage.rocks, stage.numRocks, [](const Rock& rock){return rock.id;});
	assert(rock_idx < stage.numRocks);
	return stage.rocks[rock_idx];
}
