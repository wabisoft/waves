#include <iostream>
#include <cassert>
#include "aabb.hpp"
#include "maths.hpp"
#include "printing.hpp"
#include "stage.hpp"

AABB::AABB(const Platform& platform) {
	boundingPoints(platform.shape, lower, upper);
	type = PLATFORM;
	id = platform.id;
}

AABB::AABB(const Rock& rock) {
	Vector2 diag = {rock.shape.radius, rock.shape.radius};
	Vector2 lower0 = rock.shape.position - diag;
	Vector2 upper0 = rock.shape.position + diag;
	Vector2 futurePos = rock.shape.position + rock.velocity * FIXED_TIMESTEP;
	Vector2 lower1 = futurePos - diag;
	Vector2 upper1 = futurePos + diag;
	lower = {std::min(lower0.x, lower1.x), std::min(lower0.y, lower1.y)};
	upper = {std::max(upper0.x, upper1.x), std::max(upper0.y, upper1.y)};
	type = ROCK;
	id = rock.id;
}

AABB::AABB(const Ship& ship) {
	boundingPoints(ship.shape, lower, upper);
	type = SHIP;
	id = ship.id;
}

AABB::AABB(const Sea& sea) {
	float maxHeight = sea.level;
	for(short i = 0; i < sea.numWaves; ++i) {
		maxHeight = std::max(maxHeight, heightAtX(sea.waves[i], sea.waves[i].position.x));
	}
	lower = {0.f, 0.f};
	upper = {STAGE_WIDTH, maxHeight};
	type = SEA;
	id = sea.id;
}

uint8_t createAABB(Stage& stage, AABB aabb) {
	sorted_insert(aabb, stage.aabbs, stage.numAABBS, [](AABB& a, AABB&b) { return a.lower.x < b.lower.x; });
	return aabb.id;
}

int deleteAABBByIdx(Stage& stage, int aabb_idx) {
	stage.aabbs[aabb_idx] = AABB();
	for(short j = aabb_idx; j < stage.numAABBS-1; ++j) {
		stage.aabbs[j] = stage.aabbs[j+1];
		stage.aabbs[j+1] = AABB();
	}
	return --stage.numAABBS;
}

int deleteAABBById(Stage& stage, uint8_t aabb_id) {
	for(short i = 0; i < stage.numAABBS; ++i) {
		if(stage.aabbs[i].id == aabb_id) {
			return deleteAABBByIdx(stage, i);
		}
	}
	assert(false);
	return -1;
}
