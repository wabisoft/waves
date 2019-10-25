#include <iostream>
#include <cassert>

#include <glm/vec2.hpp>


#include "aabb.hpp"
#include "maths.hpp"
#include "printing.hpp"
#include "stage.hpp"
#include "util.hpp"

void updateAABBS(Stage& stage) {
	for(AABB& aabb : stage.aabbs) {
		switch (aabb.type) {
			case Entity::ROCK:
				aabb = AABB(*findRock(stage, aabb.id));
				break;
			case Entity::SHIP:
				aabb = AABB(stage.ship);
				break;
			case Entity::SEA:
				aabb = AABB(*findSea(stage, aabb.id));
				break;
			case Entity::PLATFORM:
				aabb = AABB(*findPlatform(stage, aabb.id));
				break;
			case Entity::WIN:
				aabb = AABB(stage.win);
				break;

			default:
				break;
		}
	}
}

AABB::AABB(const Platform& platform) {
	boundingPoints(platform.shape, lower, upper);
	type = Entity::PLATFORM;
	id = platform.id;
}

AABB::AABB(const Rock& rock) {
	glm::vec2 diag = {rock.radius, rock.radius};
	glm::vec2 lower0 = rock.position - diag;
	glm::vec2 upper0 = rock.position + diag;
	glm::vec2 futurePos = rock.position + rock.velocity * FIXED_TIMESTEP;
	glm::vec2 lower1 = futurePos - diag;
	glm::vec2 upper1 = futurePos + diag;
	lower = {std::min(lower0.x, lower1.x), std::min(lower0.y, lower1.y)};
	upper = {std::max(upper0.x, upper1.x), std::max(upper0.y, upper1.y)};
	type = Entity::ROCK;
	id = rock.id;
}

AABB::AABB(const Ship& ship) {
	boundingPoints(ship.shape, lower, upper);
	type = Entity::SHIP;
	id = ship.id;
}

AABB::AABB(const Sea& sea) {
	lower = lowerBound(sea.shape);
	upper = upperBound(sea.shape);
	float maxHeight = upper.y;
	float minHeight = lower.y;
	for (const Wave& wave : sea.waves) {
		maxHeight = std::max(maxHeight, upper.y + wave.heightAtX(wave.position.x));
		minHeight = std::min(minHeight, upper.y + wave.heightAtX(wave.position.x));
	}
	upper.y = maxHeight;
	lower.y = minHeight;
	type = Entity::SEA;
	id = sea.id;
}

AABB::AABB (const Win& win) {
	boundingPoints(win.shape, lower, upper);
	type = Entity::WIN;
	id = win.id;
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
	// sorted_insert<uint8, std::vector<uint8>::iterator>(stage.xAxisOrder, aabb.id, xSortPredicate); // keep the x axis sorted
	// sorted_insert<uint8, std::vector<uint8>::iterator>(stage.yAxisOrder, aabb.id, ySortPredicate); // keep the y axis sorted
	sorted_insert(stage.xAxisOrder, aabb.id, xSortPredicate); // keep the x axis sorted
	sorted_insert(stage.yAxisOrder, aabb.id, ySortPredicate); // keep the y axis sorted
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
