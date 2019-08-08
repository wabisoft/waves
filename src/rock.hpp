#pragma once

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
	return AABB(rock.position - diag, rock.position + diag, ROCK, rock.id);
}

void fixedUpdate(Rock* rocks, size_t& numRocks);
int createRock(Stage& stage, Vector2 position, float radius);
int deleteRock(Stage& stage, int rock_id);
