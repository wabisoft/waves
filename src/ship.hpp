#pragma once

#include <glm/vec2.hpp>

#include "entity.hpp"
#include "prelude.hpp"
#include "maths.hpp"
#include "shapes.hpp"

struct Ship : Entity {
	struct State {
		struct Falling {};
		struct Standing{
			glm::vec2 surfaceStart;
			glm::vec2 surfaceEnd;
		};
		struct Surfing {
			u8 waveId;
			u8 seaId;
			int waveDirection = 0;
		};
		enum Type : u8 {
			FALLING = 1 << 0,
			STANDING = 1 << 1,
			SURFING = 1 << 2,
		};
		State::Type type;
		union {
			Falling falling;
			Standing standing;
			Surfing surfing;
		};
	};
	Ship() { }
	Ship(wabi::Polygon& polygon, glm::vec2 position, u8 id) : Entity(Entity::SHIP, polygon, position, id) { }
	Ship(wabi::Polygon&& polygon, glm::vec2 position, u8 id) : Entity(Entity::SHIP, polygon, position, id) { }

	glm::vec2 velocity = glm::vec2(0);
	float omega = 0; // rotational velocity
	float alpha = 0; // rotational acceleration
	State state = {Ship::State::FALLING, {}};
	bool inWin = false;
};

inline float mass(const Ship& ship) {
	// return area(ship.shape) * SHIP_AREA_MASS_RATIO;
	return area(ship.shape) * 112;
}

void updateShip(Stage& stage, float deltaTime);
u8 createShip(Stage& stage, glm::vec2 position, float width, float height);
u8 createShip(Stage& stage, glm::vec2 position, wabi::Polygon shape);


