#pragma once

#include "AABB.hpp"
#include "util.hpp"
#include "vector2.hpp"

struct Platform {
	Vector2 position;	
	float width;
	float height;
	int id;
};


inline AABB aabb(const Platform& platform) {
	Vector2 diag = Vector2(platform.width/2, platform.height/2);
	return AABB(platform.position - diag, platform.position + diag, PLATFORM, platform.id);
}
