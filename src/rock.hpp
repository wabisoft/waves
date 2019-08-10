#pragma once
#include <algorithm>
#include <cassert>
#include <cstdint>

#include "aabb.hpp"
#include "constants.hpp"
#include "maths.hpp"
#include "prelude.hpp"
#include "util.hpp"
#include "vector2.hpp"


struct Rock {
	Vector2 position, velocity = {0.f, 0.f};
	float radius = 0.f;
	bool active = false;
	uint8_t id = -1;
};



void updateRockAABB(AABB& a);
inline AABB aabb(const Rock& rock) {
	Vector2 diag = Vector2(rock.radius, rock.radius);
	Vector2 lower0 = rock.position - diag;
	Vector2 upper0 = rock.position + diag;
	Vector2 futurePos = rock.position + rock.velocity * FIXED_TIMESTEP;
	Vector2 lower1 = futurePos - diag;
	Vector2 upper1 = futurePos + diag;
	Vector2 lower = {std::min(lower0.x, lower1.x), std::min(lower0.y, lower1.y)};
	Vector2 upper = {std::max(upper0.x, upper1.x), std::max(upper0.y, upper1.y)};
	AABB aabb(lower, upper, ROCK, rock.id);
	aabb.entity = (void*)&rock;
	aabb.updateCallback = &updateRockAABB;
	return aabb;
}

void fixedUpdateRocks(Stage& stage);
int createRock(Stage& stage, Vector2 position, float radius);
int deleteRock(Stage& stage, uint8_t rock_id);
