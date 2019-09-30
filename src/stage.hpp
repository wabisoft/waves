#pragma once

#include <vector>
#include <string>

#include "constants.hpp"
#include "entity.hpp"
#include "platform.hpp"
#include "rock.hpp"
#include "sea.hpp"
#include "ship.hpp"

struct Selection {
	enum State : uint8_t {
		SELECT		= 1 << 0,
		PULL 		= 1 << 2,
	};
	State state = SELECT;
	Entity entity;
	Vector2 entityPosition;
	Vector2 pullPosition;
	bool active = false;
};

struct StageState {
	struct Paused {
		float time = 0.f;
	};
	struct Running {
		float time = 0.f;
	};
	struct Finished {
		bool win = false;
	};
	enum StateType : uint8_t {
		PAUSED = 1 << 0,
		RUNNING = 1 << 1,
		FINISHED = 1 << 2,
	};
	StateType type = RUNNING;
	Paused paused;
	Running running;
	Finished finished;
};

struct Win {
	float timeInArea = 0;
	float timeToWin = 0.25;
	Rectangle region;
};

struct Stage{
	Sea sea;
	Ship ship;
	std::vector<Rock> rocks;
	std::vector<Platform> platforms;
	// Platform platforms[MAX_PLATFORMS];
	AABB aabbs[MAX_AABBS];
	Vector2 rockSpawn;
	Selection selection;
	Win win;
	StageState state;
	uint8_t id_src = 0;
	int numAABBS = 0;
	RockType rockType = {RockType::RED};
};

void update(Stage& stage, float deltaTime);
Entity makeSelectionAtPosition(Stage& stage, Vector2 position);
void clearSelection(Stage& stage);
void processStartInput(Stage& stage, Vector2 position);
void processContinuingInput(Stage& stage, Vector2 position);
void processEndInput(Stage& stage, Vector2 position);
Vector2 getPullForce(Stage& stage);
std::vector<Vector2> pullParabola(Stage& stage);

inline bool validateAndSetPullPosition(Stage& stage, Vector2 position);
inline bool outOfBounds(Vector2 position) {
	return (position.x<0 ||position.x > STAGE_WIDTH || position.y < 0);
}


