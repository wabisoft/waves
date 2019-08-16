#pragma once
#include <algorithm>
#include <cassert>
#include <cstdint>

#include "aabb.hpp"
#include "constants.hpp"
#include "maths.hpp"
#include "prelude.hpp"
#include "util.hpp"


enum RockState : uint8_t {
	STANDING = 1 << 0, // to launch and stuff
	FALLING = 1 << 1, // to fall and bounce and things
};

struct Rock {
	Circle shape = {{0.f, 0.f}, 0.f};
	Vector2 velocity = {0.f, 0.f};
	bool active = false;
	uint8_t id = -1;
	RockState state = FALLING;
};

void fixedUpdateRocks(Stage& stage);
uint8_t createRock(Stage& stage, Vector2 position, float radius);
size_t deleteRockByIdx(Stage& stage, int rock_idx);
size_t deleteRockById(Stage& stage, uint8_t rock_id);
Rock& findRock(Stage& stage, uint8_t rock_id);

inline float area(const Rock& rock) {
	return rock.shape.radius * rock.shape.radius * PI;
}
