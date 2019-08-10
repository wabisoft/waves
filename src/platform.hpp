#pragma once
#include "aabb.hpp"
#include "util.hpp"
#include "vector2.hpp"

struct Platform {
	Vector2 position = {0.f, 0.f};
	float width = 0.f;
	float height = 0.f;
	unsigned char id = 0;
	static unsigned char id_src;
};



inline AABB aabb(const Platform& platform) {
	Vector2 diag = Vector2(platform.width/2, platform.height/2);
	AABB aabb(platform.position - diag, platform.position + diag, PLATFORM, platform.id);
	aabb.entity = (void*)&platform;
	return aabb;
}
