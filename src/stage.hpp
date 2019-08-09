#pragma once

#include <vector>

#include "constants.hpp"
#include "platform.hpp"
#include "rock.hpp"
#include "sea.hpp"
#include "ship.hpp"

struct Stage{	
	inline Stage(float sea_level): sea(sea_level) { }
	Sea sea;
	Ship ship;
	Rock rocks[MAX_ROCKS];
	Platform platforms[MAX_PLATFORMS];
	int id_src = 0;
	size_t numRocks = 0;
	size_t numPlatforms = 0;
	bool paused = false;
};

// void dispatchEvent(const Event& event){
	// TODO: dispatch event and do things
// }
//

void fixedUpdate(Stage& stage, float deltaTime);	
