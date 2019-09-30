#include <iostream>
#include <cassert>
#include "aabb.hpp"
#include "maths.hpp"
#include "printing.hpp"
#include "stage.hpp"
void updateAABBS(Stage& stage) {
	for(AABB& aabb : stage.aabbs) {
		switch (aabb.type) {
			case ROCK:
				aabb = AABB(*findRock(stage, aabb.id));
				break;
			case SHIP:
				aabb = AABB(stage.ship);
				break;
			case PLATFORM:
				aabb = AABB(*findPlatform(stage, aabb.id));
				break;
			default:
				break;
		}
	}
}

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
	sorted_insert<AABB, AABBIt>(stage.aabbs, aabb, [](AABB a, AABB b) { return a.lower.x < b.lower.x; });
	return aabb.id;
}

AABBIt findAABB(Stage& stage, uint8 aabbId) {
	AABBIt search = std::find_if(stage.aabbs.begin(), stage.aabbs.end(), [aabbId](AABB& aabb) -> bool { return aabb.id == aabbId; });
	assert(search != stage.aabbs.end()); // don't look for unicorns!
	return search;
}

AABBIt deleteAABB(Stage& stage, AABBIt aabbIt) {
	return stage.aabbs.erase(aabbIt);
}

AABBIt deleteAABB(Stage& stage, uint8 aabbId) {
	return stage.aabbs.erase(findAABB(stage, aabbId));
}
