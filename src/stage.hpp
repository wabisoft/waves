#pragma once

#include <vector>

#include "constants.hpp"
#include "entity.hpp"
#include "platform.hpp"
#include "rock.hpp"
#include "sea.hpp"
#include "ship.hpp"

struct Selection {
	Entity entity;
	bool active = false;
};

enum Phase : uint8_t {
	RESIZE 		= 1 << 0,
	PREPULL 	= 1 << 1,
	PULL 		= 1 << 2,
};

struct Stage{
	Sea sea;
	Ship ship;
	Rock rocks[MAX_ROCKS];
	Platform platforms[MAX_PLATFORMS];
	AABB aabbs[MAX_AABBS];
	Vector2 rockSpawn;
	Vector2 pullPosition; // the position of the throw pull (just used for drawing)
	Selection selection;
	Phase phase = RESIZE;
	uint8_t id_src = 0;
	int numRocks = 0;
	int numPlatforms = 0;
	int numAABBS = 0;
	bool paused = false;
};

// void dispatchEvent(const Event& event){
	// TODO: dispatch event and do things
// }
//

void update(Stage& stage, float deltaTime);
Entity selectEntityAtPosition(Stage& stage, Vector2 position);
void processStartInput(Stage& stage, Vector2 position);
void processContinuingInput(Stage& stage, Vector2 position);
void processEndInput(Stage& stage, Vector2 position);

inline bool validateAndSetPullPosition(Stage& stage, Vector2 position);
