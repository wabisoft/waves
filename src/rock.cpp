#include <iostream>

#include "collision.hpp"
#include "constants.hpp"
#include "gravity.hpp"
#include "rock.hpp"
#include "stage.hpp"

void updateRockAABB(AABB& a) {
	assert(a.type & ROCK); // XXX: Assert the type of the aabb
	a = aabb(*(Rock*)a.entity);
}

void fixedUpdateRocks(Stage& stage) {
	Rock* rocks = stage.rocks;
	for (short i = 0; i < stage.numRocks; ++i) {
		if (rocks[i].position.x < 0 || rocks[i].position.x > STAGE_WIDTH || rocks[i].position.y < 0 || rocks[i].position.y > STAGE_HEIGHT || rocks[i].active == false) {
			// remove our aabb from the stage aabb array
			for(short k = 0; k < stage.numAABBS; ++k) { // look for the aabb
				if(stage.aabbs[k].id == rocks[i].id) { // find the aabb
					stage.aabbs[k] = AABB(); // "reset" the aabb;
					for(short l = k; l < stage.numAABBS; ++l) { // 
						stage.aabbs[l] = stage.aabbs[l+1]; // defrag the array
					}
					--stage.numAABBS;
				}
			}
			// now "reset this rock" 
			rocks[i] = Rock();
			for (short j = i; j < stage.numRocks-1; j++){
				rocks[j] = rocks[j+1];
			}
			--stage.numRocks;
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
	short new_rock_idx = stage.numRocks;
	Rock& new_rock = *(stage.rocks + new_rock_idx);
	new_rock = Rock();
	new_rock.active = true;
	new_rock.id = ++stage.id_src;
	new_rock.position = position;
	new_rock.radius = radius;
	stage.numRocks++;
	sorted_insert(aabb(new_rock), stage.aabbs, stage.numAABBS, [](AABB& a, AABB&b) { return a.lower.x < b.lower.x; });
	return new_rock.id;
}

int deleteRock(Stage& stage, unsigned char rock_id){
	short rock_idx = -1;
	Rock * rocks = stage.rocks;
	for(short i = 0; i < stage.numRocks; i++){
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


