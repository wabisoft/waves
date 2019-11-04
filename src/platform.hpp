#pragma once

#include <glm/vec2.hpp>

#include "aabb.hpp"
#include "constants.hpp"
#include "entity.hpp"
#include "maths.hpp"
#include "shapes.hpp"
#include "typedefs.hpp"
#include "util.hpp"

struct Platform : Entity {
	Platform() {}
	Platform(wabi::Polygon shape, glm::vec2 position, uint8 id) : Entity(Entity::PLATFORM, shape, position, id) { }
};

typedef std::vector<Platform>::iterator PlatformIt;

uint8 createPlatform(Stage& stage, glm::vec2 position, float width, float height);
uint8 createPlatform(Stage& stage, glm::vec2 position, wabi::Polygon&);
PlatformIt findPlatform(Stage& stage, uint8 platform_id);

inline float mass(const Platform& platform) {
	// return area(platform.shape) * PLATFORM_AREA_MASS_RATIO;
	return area(platform.shape) * 112;
}
