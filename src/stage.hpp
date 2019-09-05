#pragma once

#include <vector>

#include "constants.hpp"
#include "entity.hpp"
#include "platform.hpp"
#include "rock.hpp"
#include "sea.hpp"
#include "ship.hpp"

struct Selection {
	enum State : uint8_t {
		SELECT		= 1 << 0,
		RESIZE 		= 1 << 1,
		PULL 		= 1 << 2,
	};
	State state = SELECT;
	Entity entity;
	Vector2 entityPosition;
	Vector2 pullPosition;
	bool active = false;
};

struct WinCondition {
	int timeInAreaForWin = 0;
	Rectangle winArea;
};

struct Stage{
	Sea sea;
	Ship ship;
	Rock rocks[MAX_ROCKS];
	Platform platforms[MAX_PLATFORMS];
	AABB aabbs[MAX_AABBS];
	// std::vector<AABBPair> aabbPairs;
	Vector2 rockSpawn;
	// Vector2 pullPosition; // the position of the throw pull (just used for drawing)
	Selection selection;
	uint8_t id_src = 0;
	int numRocks = 0;
	int numPlatforms = 0;
	int numAABBS = 0;
	int rockLimit = 0;
	int usedRocks = 0;
	bool paused = false;
	bool failed = false;
};

void update(Stage& stage, float deltaTime);
Entity makeSelectionAtPosition(Stage& stage, Vector2 position);
void clearSelection(Stage& stage);
void processStartInput(Stage& stage, Vector2 position);
void processContinuingInput(Stage& stage, Vector2 position);
void processEndInput(Stage& stage, Vector2 position);

inline bool validateAndSetPullPosition(Stage& stage, Vector2 position);
