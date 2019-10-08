#pragma once
#include <algorithm>
#include <cassert>
#include <cstdint>

#include "aabb.hpp"
#include "constants.hpp"
#include "maths.hpp"
#include "prelude.hpp"
#include "shapes.hpp"
#include "typedefs.hpp"
#include "util.hpp"


struct RockState {
	struct FallingState { };
	struct StandingState{
		Vector2 surfaceStart;
		Vector2 surfaceEnd;
	};
	struct FloatingState {
		float timeFloating = 0.f;
		uint8 seaId = 0;
	};

	enum StateType : uint8 {
		FALLING = 1 << 0,
		STANDING = 1 << 1,
		FLOATING = 1 << 2,
	};
	StateType type;
	union {
		StandingState standing;
		FallingState falling;
		FloatingState floating;
	};
};

struct RockType {
	struct RedType {};
	struct GreenType {};
	struct BlueType {};
	enum TypeType{
		RED,
		GREEN,
		BLUE
	};
	TypeType type;
	union {
		RedType red;
		GreenType green;
		BlueType blue;
	};
};


struct Rock {
	wabi::Circle shape = {{0.f, 0.f}, 0.f};
	Vector2 velocity = {0.f, 0.f};
	RockState state { RockState::FALLING, {} };
	RockType type;
	bool active = false;
	bool sized = false;
	uint8 id = 0;
};

typedef std::vector<Rock>::iterator RockIt;

inline float mass(Rock& rock) {
	return rock.shape.radius * ROCK_RADIUS_MASS_RATIO;
}

void updateRocks(Stage& stage, float deltaTime);
uint8 createRock(Stage& stage, Vector2 position, float radius, RockType type);
RockIt deleteRock(Stage& stage, RockIt rockIt);
RockIt deleteRock(Stage& stage, uint8 rockId);
RockIt findRock(Stage& stage, uint8 rock_id);
RockIt findRockAtPosition(Stage& stage, Vector2 position);
void resizeRock(Stage& stage, int rockId, Vector2 position);

inline float area(const Rock& rock) {
	return rock.shape.radius * rock.shape.radius * PI;
}
