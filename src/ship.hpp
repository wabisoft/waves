#pragma once

#include <glm/vec2.hpp>

#include "prelude.hpp"
#include "maths.hpp"
#include "typedefs.hpp"
#include "shapes.hpp"

struct ShipState {
	struct FallingState { };
	struct StandingState{
		glm::vec2 surfaceStart;
		glm::vec2 surfaceEnd;
	};
	struct SurfingState {
		uint8 waveId;
		uint8 seaId;
		int waveDirection = 0;
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
	wabi::Rectangle shape;
	glm::vec2 velocity;
	ShipState state = {ShipState::FALLING, {}};
	uint8_t id = 0;
	bool active = false;
};

inline float mass(const Ship& ship) {
	return area(ship.shape) * SHIP_AREA_MASS_RATIO;
}

void updateShip(Stage& stage, float deltaTime);
uint8_t createShip(Stage& stage, glm::vec2 position, float width, float height);
uint8_t createShip(Stage& stage, wabi::Rectangle rect);


