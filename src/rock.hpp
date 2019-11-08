#pragma once
#include <algorithm>
#include <cassert>
#include <cstdint>

#include <glm/vec2.hpp>

#include "aabb.hpp"
#include "settings.hpp"
#include "maths.hpp"
#include "prelude.hpp"
#include "shapes.hpp"
#include "util.hpp"


struct Rock : Entity {
	struct State {
		struct Falling {};
		struct Standing{
			glm::vec2 surfaceStart;
			glm::vec2 surfaceEnd;
		};
		struct Floating {
			float time = 0.f;
			u8 seaId = 0;
		};
		struct Sinking {
			float time = 0.f;
		};
		enum Type : u8 {
			FALLING = 1 << 0,
			STANDING = 1 << 1,
			FLOATING = 1 << 2,
			SINKING = 1 << 3,
		};
		State::Type type;
		union {
			Falling falling;
			Standing standing;
			Floating floating;
			Sinking	sinking;
		};
	};
	enum Kind {
		RED,
		GREEN,
		BLUE
	};

	Rock() { }
	Rock(wabi::Polygon shape, glm::vec2 position, u8 id, float radius, Rock::Kind kind)
		: Entity(Entity::ROCK, shape, position, id), radius(radius), kind(kind) {}

	glm::vec2 velocity = glm::vec2(0);
	Rock::State state { Rock::State::FALLING, {} };
	Rock::Kind kind;
	float radius = 0;
};

typedef std::vector<Rock>::iterator RockIt;

inline float mass(Rock& rock) {
	return rock.radius * ROCK_RADIUS_MASS_RATIO;
}

void updateRocks(Stage& stage, float deltaTime);
u8 createRock(Stage& stage, glm::vec2 position, float radius, Rock::Kind kind);
u8 createRock(Stage& stage, const wabi::Polygon& polygon, glm::vec2 position, float radius, Rock::Kind kind);
RockIt deleteRock(Stage& stage, RockIt rockIt);
RockIt deleteRock(Stage& stage, u8 rockId);
RockIt findRock(Stage& stage, u8 rock_id);
RockIt findRockAtPosition(Stage& stage, glm::vec2 position);
// void resizeRock(Stage& stage, int rockId, glm::vec2 position);

inline float area(const Rock& rock) {
	return area(rock.shape);
}
