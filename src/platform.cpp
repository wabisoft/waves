#include "typedefs.hpp"
#include "platform.hpp"
#include "stage.hpp"

using namespace wabi;

uint8 createPlatform(Stage& stage, Vector2 position, float width, float height) {
	Platform platform;
	platform.shape = makeRectangle(position, width, height);
	platform.id = ++stage.id_src;
	stage.platforms.push_back(platform);
	createAABB(stage, AABB(platform));
	return platform.id;
}

uint8 createPlatform(Stage& stage, Rectangle rect) {
	Platform platform;
	platform.shape = rect;
	platform.id = ++stage.id_src;
	stage.platforms.push_back(platform);
	createAABB(stage, AABB(platform));
	return platform.id;
}

PlatformIt findPlatform(Stage& stage, uint8 platformId) {
	PlatformIt platformIt = std::lower_bound(stage.platforms.begin(), stage.platforms.end(), platformId, [](Platform& p, uint8 id) -> bool { return p.id < id; });
	assert(platformIt != stage.platforms.end());
	return platformIt;
}
