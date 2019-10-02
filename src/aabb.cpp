#include <iostream>
#include <cassert>
#include "aabb.hpp"
#include "maths.hpp"
#include "printing.hpp"
#include "stage.hpp"
#include "util.hpp"

void updateAABBS(Stage& stage) {
	for(AABB& aabb : stage.aabbs) {
		switch (aabb.type) {
			case ROCK:
				aabb = AABB(*findRock(stage, aabb.id));
				break;
			case SHIP:
				aabb = AABB(stage.ship);
				break;
			case SEA:
				aabb = AABB(*findSea(stage, aabb.id));
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
	float maxHeight = sea.shape.height;
	for (const Wave& wave : sea.waves) {
		maxHeight = std::max(maxHeight, sea.shape.height + wave.heightAtX(wave.position.x));
	}
	lower = lowerBound(sea.shape);
	upper = {upperBound(sea.shape).x, maxHeight};
	type = SEA;
	id = sea.id;
}

uint8_t createAABB(Stage& stage, AABB aabb) {
	stage.aabbs.push_back(aabb);
	// TODO: make these predicates named functions or cache the lambdas someplace
	auto xSortPredicate = [&stage](int aId, int bId) {
		AABB a = *findAABB(stage, aId);
		AABB b = *findAABB(stage, bId);
		return a.lower.x < b.lower.x;
	};
	auto ySortPredicate = [&stage](int aId, int bId) {
		AABB a = *findAABB(stage, aId);
		AABB b = *findAABB(stage, bId);
		return a.lower.y < b.lower.y;
	};
	sorted_insert<uint8, std::vector<uint8>::iterator>(stage.xAxisOrder, aabb.id, xSortPredicate); // keep the x axis sorted
	sorted_insert<uint8, std::vector<uint8>::iterator>(stage.yAxisOrder, aabb.id, ySortPredicate); // keep the y axis sorted
	return aabb.id;
}

AABBIt findAABB(Stage& stage, uint8 aabbId) {
	AABBIt search = std::find_if(stage.aabbs.begin(), stage.aabbs.end(), [aabbId](AABB& aabb) -> bool { return aabb.id == aabbId; });
	assert(search != stage.aabbs.end()); // don't look for unicorns!
	return search;
}

AABBIt deleteAABB(Stage& stage, AABBIt aabbIt) {
	uint8 aabbId = aabbIt->id;
	// look for id in xAxisOrdering
	auto search = find_if(stage.xAxisOrder.begin(), stage.xAxisOrder.end(), [aabbId](uint8 id) -> bool { return aabbId == id; });	
	assert(search != stage.xAxisOrder.end()); // don't look for unicorns!
	// delete from xAxisOrdering
	stage.xAxisOrder.erase(search);
	// look for id in yAxisOrdering
	search = find_if(stage.yAxisOrder.begin(), stage.yAxisOrder.end(), [aabbId](uint8 id) -> bool { return aabbId == id; });	
	assert(search != stage.yAxisOrder.end()); // don't look for unicorns!
	// delete from yAxisOrdering
	stage.yAxisOrder.erase(search);
	return stage.aabbs.erase(aabbIt);
}

AABBIt deleteAABB(Stage& stage, uint8 aabbId) {
	return deleteAABB(stage, findAABB(stage, aabbId));
}
