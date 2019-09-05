#pragma once

#include "prelude.hpp"
#include "maths.hpp"

struct ShipState {
	struct FallingState { };
	struct StandingState{
		Vector2 surfaceStart;
		Vector2 surfaceEnd;
	};
	struct SurfingState {
		uint8_t wave_id;
	};

	enum StateType : uint8_t {
		FALLING = 1 << 0,
		STANDING = 1 << 1,
		SURFING = 1 << 2,
	};

	StateType type;
	union {
		StandingState standing;
		FallingState falling;
		SurfingState surfing;
	};
};

struct Ship {
	Rectangle shape;
	Vector2 velocity;
	ShipState state = {ShipState::FALLING, {}};
	uint8_t id = 0;
	bool active = false;
};

inline float mass(const Ship& ship) {
	return area(ship.shape) * SHIP_AREA_MASS_RATIO;
}

void updateShip(Stage& stage);
uint8_t createShip(Stage& stage, Vector2 position, float width, float height);


