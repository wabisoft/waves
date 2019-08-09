#pragma once

#include <cmath>
#include "prelude.hpp"
#include "constants.hpp"
#include "aabb.hpp"
#include "util.hpp"
#include "vector2.hpp"


struct Rock {
	// Rock() {}
	// explicit Rock(const Rock& rock) : 
	// 	position(rock.position),
	// 	velocity(rock.velocity),
	// 	radius(rock.radius),
	// 	id(rock.id),
	// 	active(rock.active) {}	
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
	Vector2 lower = {std::min(lower0.x, lower1.x), std::min(lower0.y, lower1.y)};
	Vector2 upper = {std::max(upper0.x, upper1.x), std::max(upper0.y, upper1.y)};
	return AABB(lower, upper, ROCK, rock.id);
}

void fixedUpdateRocks(Stage& stage);
int createRock(Stage& stage, Vector2 position, float radius);
int deleteRock(Stage& stage, int rock_id);
