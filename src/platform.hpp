#pragma once
#include "aabb.hpp"
#include "util.hpp"
#include "vector2.hpp"

struct Platform {
	Vector2 position = {0.f, 0.f};
	float width = 0.f;
	float height = 0.f;
	uint8_t id = 0;
};

uint8_t createPlatform(Stage& stage, Vector2 position, float width, float height);
Platform& findPlatform(Stage& stage, uint8_t platform_id);
