#pragma once

#include <glm/vec2.hpp>

#include "aabb.hpp"
#include "constants.hpp"
#include "maths.hpp"
#include "shapes.hpp"
#include "typedefs.hpp"
#include "util.hpp"

struct Platform {
	wabi::Rectangle shape;
	uint8 id = 0;
};

typedef std::vector<Platform>::iterator PlatformIt;

uint8 createPlatform(Stage& stage, glm::vec2 position, float width, float height);
uint8 createPlatform(Stage& stage, wabi::Rectangle);
PlatformIt findPlatform(Stage& stage, uint8 platform_id);

inline float mass(const Platform& platform) {
	return area(platform.shape) * PLATFORM_AREA_MASS_RATIO;
}
