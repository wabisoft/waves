#pragma once

#include "aabb.hpp"
#include "constants.hpp"
#include "maths.hpp"
#include "prelude.hpp"
#include "util.hpp"
#include "vector2.hpp"


struct Rock {
	Vector2 position, velocity = {0.f, 0.f};
	float radius = 0.f;
	int id = -1;
	bool active = false;
};


inline AABB aabb(const Rock& rock) {
	Vector2 diag = Vector2(rock.radius, rock.radius);
	Vector2 lower0 = rock.position - diag;
	Vector2 upper0 = rock.position + diag;
	Vector2 futurePos = rock.position + rock.velocity * FIXED_TIMESTEP;
	Vector2 lower1 = futurePos - diag;
	Vector2 upper1 = futurePos + diag;
	Vector2 lower = {std::fmin(lower0.x, lower1.x), std::fmin(lower0.y, lower1.y)};
	Vector2 upper = {std::fmax(upper0.x, upper1.x), std::fmax(upper0.y, upper1.y)};
	AABB aabb(lower, upper, ROCK, rock.id);
	aabb.entity = (void*)&rock;
	return aabb;
}

void fixedUpdateRocks(Stage& stage);
int createRock(Stage& stage, Vector2 position, float radius);
int deleteRock(Stage& stage, int rock_id);
