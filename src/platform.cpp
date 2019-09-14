#include "platform.hpp"
#include "stage.hpp"

uint8_t createPlatform(Stage& stage, Vector2 position, float width, float height) {
	if (stage.numPlatforms >= MAX_PLATFORMS) {
		return -1;
	}
	int new_platform_idx = stage.numPlatforms;
	Platform& platform = *(stage.platforms + new_platform_idx);
	platform = Platform();
	platform.shape = makeRectangle(position, width, height);
	platform.id = ++stage.id_src;
	stage.numPlatforms++;
	createAABB(stage, AABB(platform));
	return platform.id;
}

uint8_t createPlatform(Stage& stage, Rectangle rect) {
	if (stage.numPlatforms >= MAX_PLATFORMS) {
		return -1;
	}
	int new_platform_idx = stage.numPlatforms;
	Platform& platform = *(stage.platforms + new_platform_idx);
	platform = Platform();
	platform.shape = rect;
	platform.id = ++stage.id_src;
	stage.numPlatforms++;
	createAABB(stage, AABB(platform));
	return platform.id;
}

Platform& findPlatform(Stage& stage, uint8_t platform_id) {
	int platform_idx = binary_find_where(platform_id, stage.platforms, stage.numPlatforms, [](const Platform& platform) {return platform.id;});
	// NOTE: binary_find_where will return -1 if it doesn't find the id you ask for
	// this means that if you look for a platform that isn't there ...
	assert(platform_idx > -1); // ... then this assertion will fail ...
	// ... and you will die
	// So don't go lookin for no imaginary platforms
	return stage.platforms[platform_idx];
}
