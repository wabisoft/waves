#pragma once
#include "aabb.hpp"
#include "constants.hpp"
#include "maths.hpp"
#include "util.hpp"

struct Platform {
	Rectangle shape;
	uint8_t id = 0;
};

uint8_t createPlatform(Stage& stage, Vector2 position, float width, float height);
uint8_t createPlatform(Stage& stage, Rectangle);
Platform& findPlatform(Stage& stage, uint8_t platform_id);
inline float mass(const Platform& platform) {
	return area(platform.shape) * PLATFORM_AREA_MASS_RATIO;
}
