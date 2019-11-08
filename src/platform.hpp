#pragma once

#include <glm/vec2.hpp>

#include "aabb.hpp"
#include "settings.hpp"
#include "entity.hpp"
#include "maths.hpp"
#include "shapes.hpp"

struct Platform : Entity {
	Platform() {}
	Platform(wabi::Polygon shape, glm::vec2 position, u8 id) : Entity(Entity::PLATFORM, shape, position, id) { }
};

typedef std::vector<Platform>::iterator PlatformIt;

u8 createPlatform(Stage& stage, glm::vec2 position, float width, float height);
u8 createPlatform(Stage& stage, glm::vec2 position, wabi::Polygon&);
PlatformIt findPlatform(Stage& stage, u8 platform_id);

inline float mass(const Platform& platform) {
	// return area(platform.shape) * PLATFORM_AREA_MASS_RATIO;
	return area(platform.shape) * 112;
}
