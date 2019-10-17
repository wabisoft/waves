#pragma once

#include <vector>
#include <string>

#include <glm/vec2.hpp>

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
		glm::vec2 pullPosition = VEC2_ZERO;
	};
	struct RepositionState {
		glm::vec2 repositionPosition = VEC2_ZERO;
	};

	State state = SELECT;
	Entity entity;
	glm::vec2 entityPosition;

	PullState pull;
	RepositionState reposition;

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

	glm::vec2 rockSpawn;
	Selection selection;
	Win win;
	StageState state;
	uint8 id_src = 0;
	RockType rockType = {RockType::RED};
};

void update(Stage& stage, float deltaTime);
Entity makeSelectionAtPosition(Stage& stage, glm::vec2 position);
void clearSelection(Stage& stage);
void processStartInput(Stage& stage, glm::vec2 position);
void processContinuingInput(Stage& stage, glm::vec2 position);
void processEndInput(Stage& stage, glm::vec2 position);
glm::vec2 getPullForce(Stage& stage);
std::vector<glm::vec2> pullParabola(Stage& stage);

inline bool validateAndSetPullPosition(Stage& stage, glm::vec2 position);
inline bool outOfBounds(glm::vec2 position) {
	return (position.x<0 ||position.x > STAGE_WIDTH || position.y < 0);
}


