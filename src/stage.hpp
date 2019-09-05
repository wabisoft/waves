#pragma once

#include <vector>

#include "constants.hpp"
#include "entity.hpp"
#include "platform.hpp"
#include "rock.hpp"
#include "sea.hpp"
#include "ship.hpp"

struct SelectionState {
	struct SelectState { };
	struct ResizeState {
		Entity entity;
		Vector2 position;
	};
	struct PullState{
		Entity entity;
		Vector2 entityPosition;
		Vector2 pullPosition;
	};

	enum StateType : uint8_t {
		SELECT		= 1 << 0,
		RESIZE 		= 1 << 1,
		PULL 		= 1 << 2,
	};
	StateType type;
	union {
		SelectState select;
		ResizeState resize;
		PullState pull;
	};
};

struct Selection {
	Entity entity;
	bool active = false;
	SelectionState state = { SelectionState::SELECT, {} };
};

enum Phase : uint8_t {
	SELECT		= 1 << 0, // XXX: I think we can get away with just SELECT, RESIZE and PULL if we hold a sized flag in Rock
	RESIZE 		= 1 << 1,
	PULL 		= 1 << 2,
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
	// Phase phase = SELECT;
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
