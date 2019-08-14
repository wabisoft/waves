#include "platform.hpp"
#include "stage.hpp"

uint8_t createPlatform(Stage& stage, Vector2 position, float width, float height) {
	if (stage.numPlatforms >= MAX_PLATFORMS) {
		return -1;
	}
	size_t new_platform_idx = stage.numPlatforms;	
	Platform& platform = *(stage.platforms + new_platform_idx);
	platform = Platform();
	platform.id = ++stage.id_src;
	platform.position = position;
	platform.width = width;
	platform.height = height;
	stage.numPlatforms++;
	createAABB(stage, AABB(platform));
	return platform.id;
}

Platform& findPlatform(Stage& stage, uint8_t platform_id) {
	size_t platform_idx = binary_find_where(platform_id, stage.platforms, stage.numPlatforms, [](const Platform& platform) {return platform.id;});
	assert(platform_idx < stage.numPlatforms);
	return stage.platforms[platform_idx];
}
