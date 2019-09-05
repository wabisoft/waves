#pragma once
#include <algorithm>
#include <cassert>
#include <cstdint>

#include "aabb.hpp"
#include "constants.hpp"
#include "maths.hpp"
#include "prelude.hpp"
#include "util.hpp"


struct RockState {
	struct FallingState { };
	struct StandingState{
		Vector2 surfaceStart;
		Vector2 surfaceEnd;
	};
	enum StateType : uint8_t {
		FALLING = 1 << 0,
		STANDING = 1 << 1,
	};
	StateType type;
	union {
		StandingState standing;
		FallingState falling;
	};
};

struct Rock {
	Circle shape = {{0.f, 0.f}, 0.f};
	Vector2 velocity = {0.f, 0.f};
	RockState state { RockState::FALLING, {} };
	bool active = false;
	bool sized = false;
	uint8_t id = 0;
};

inline float mass(Rock& rock) {
	return rock.shape.radius * ROCK_RADIUS_MASS_RATIO;
}

void updateRocks(Stage& stage);
uint8_t createRock(Stage& stage, Vector2 position, float radius);
int deleteRockByIdx(Stage& stage, int rock_idx);
int deleteRockById(Stage& stage, uint8_t rock_id);
Rock& findRock(Stage& stage, uint8_t rock_id);
Entity findRockAtPosition(Stage& stage, Vector2 position);
void resizeRock(Stage& stage, int rockId, Vector2 position);

inline float area(const Rock& rock) {
	return rock.shape.radius * rock.shape.radius * PI;
}
