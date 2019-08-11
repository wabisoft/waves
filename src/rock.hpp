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

void fixedUpdateRocks(Stage& stage);
int createRock(Stage& stage, Vector2 position, float radius);
int deleteRockByIdx(Stage& stage, int rock_idx);
int deleteRockById(Stage& stage, uint8_t rock_id);
Rock& findRock(Stage& stage, uint8_t rock_id);
