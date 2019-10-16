#pragma once

#include <vector>
#include <string>

#include "constants.hpp"
#include "entity.hpp"
#include "platform.hpp"
#include "rock.hpp"
#include "sea.hpp"
#include "shapes.hpp"
#include "ship.hpp"
#include "typedefs.hpp"

struct Selection {
	enum State : uint8 {
		SELECT,
		PULL,
	};

	struct PullState {
		Vector2 pullPosition = VECTOR2_ZERO;
	};
	struct RepositionState {
		Vector2 repositionPosition = VECTOR2_ZERO;
	};

	State state = SELECT;
	Entity entity;
	Vector2 entityPosition;

	PullState pull;
	RepositionState reposition;

	bool active = false;
};

inline const char * str(Selection::State s) {
	const char * names[] = {
		"SELECT", "PULL"
	};
	return names[s];
}

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

	enum StateType : uint8 {
		PAUSED,
		RUNNING,
		FINISHED,
	};

	StateType type = RUNNING;
	Paused paused;
	Running running;
	Finished finished;
};

struct Win {
	float timeInArea = 0;
	float timeToWin = 0.25;
	wabi::Rectangle region;
};

struct Stage{
	// Sea sea;
	Ship ship;
	std::vector<Sea> seas;
	std::vector<Rock> rocks;
	std::vector<Platform> platforms;
	// TODO: move this shit to a struct just for collision stuff maybe? gonna make addressing it longer though
	std::vector<AABB> aabbs;
	std::vector<uint8> xAxisOrder;
	std::vector<uint8> yAxisOrder;

	Vector2 rockSpawn;
	Selection selection;
	Win win;
	StageState state;
	uint8 id_src = 0;
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


