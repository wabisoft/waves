#pragma once
#include "aabb.hpp"
#include "maths.hpp"
#include "util.hpp"

struct Platform {
	Rectangle shape;
	uint8_t id = 0;
};

uint8_t createPlatform(Stage& stage, Vector2 position, float width, float height);
Platform& findPlatform(Stage& stage, uint8_t platform_id);
