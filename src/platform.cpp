#include "typedefs.hpp"
#include "platform.hpp"
#include "stage.hpp"

using namespace wabi;
using namespace glm;

uint8 createPlatform(Stage& stage, vec2 position, float width, float height) {
	Platform platform = Platform(makeRectangle(width, height), position, ++stage.id_src);
	stage.platforms.push_back(platform);
	createAABB(stage, AABB(platform));
	return platform.id;
}

uint8 createPlatform(Stage& stage, vec2 position, Polygon& polygon) {
	Platform platform = Platform(polygon, position, ++stage.id_src);
	stage.platforms.push_back(platform);
	createAABB(stage, AABB(platform));
	return platform.id;
}

PlatformIt findPlatform(Stage& stage, uint8 platformId) {
	PlatformIt platformIt = std::lower_bound(stage.platforms.begin(), stage.platforms.end(), platformId, [](Platform& p, uint8 id) -> bool { return p.id < id; });
	assert(platformIt != stage.platforms.end());
	return platformIt;
}
