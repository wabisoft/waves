#pragma once

#include <glm/vec2.hpp>

#include "entity.hpp"
#include "prelude.hpp"
#include "maths.hpp"
#include "typedefs.hpp"
#include "shapes.hpp"

struct Ship : Entity {
	struct State {
		struct Falling {};
		struct Standing{
			glm::vec2 surfaceStart;
			glm::vec2 surfaceEnd;
		};
		struct Surfing {
			uint8 waveId;
			uint8 seaId;
			int waveDirection = 0;
		};
		enum Type : uint8_t {
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
	Ship(wabi::Polygon& polygon, glm::vec2 position, uint8 id) : Entity(Entity::SHIP, polygon, position, id) { }

	glm::vec2 velocity = glm::vec2(0);
	State state = {Ship::State::FALLING, {}};
	bool inWin;
};

inline float mass(const Ship& ship) {
	return area(ship.shape) * SHIP_AREA_MASS_RATIO;
}

void updateShip(Stage& stage, float deltaTime);
uint8_t createShip(Stage& stage, glm::vec2 position, float width, float height);
uint8_t createShip(Stage& stage, glm::vec2 position, wabi::Polygon shape);


