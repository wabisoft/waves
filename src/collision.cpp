#include <cassert>

#include "entity.hpp"
#include "collision.hpp"
#include "util.hpp"

void resolveCollisions(Stage& stage) {
	// NOTE (owen): if it starts to get slow this is definately a place we can optimize
	//
	// OPTMZ: keep aabbs attached to stage struct and manage it with the lifetime of other objects?

	AABB aabbs[MAX_AABBS];
	size_t numAABBs = 0;
	auto aabbSortPredicate = [](AABB a, AABB b) { return a.lower.x < b.lower.x; };
	for (size_t i = 0; i < stage.numRocks; ++i) {
		sorted_insert(aabb(stage.rocks[i]), aabbs, numAABBs, aabbSortPredicate);
	}
	for(size_t i = 0; i < stage.numPlatforms; ++i){
		sorted_insert(aabb(stage.platforms[i]), aabbs, numAABBs, aabbSortPredicate);
	}
	// sorted_insert(aabb(stage.ship), aabbs, numAABBs, aabbSortPredicate);
	// sorted_insert(aabb(stage.sea), aabbs, numAABBs, aabbSortPredicate);

	// start with the first aabb upper as the max
	float max = aabbs[0].upper.x;
	// Loop the aabbs and look for collisions
	for (size_t i = 1; i < numAABBs; ++i){
		for (size_t j = i-1; j > 0; --j) {
			if (aabbs[j].upper.x > aabbs[i].lower.x) {
				// OPTMZ: maybe push to a y array in sorted order and then run the sweep on that array again? (optimization)
				dispatchPotentialCollision(stage, aabbs[j], aabbs[i]);
			}
			// if the current collider is right of the right most point of our
			// biggest-so-far box then we don't need to keep looking bac
			if (aabbs[i].lower.x > max) break;
		}
		// if this box's rect is the biggest we've seen so far then remember it
		max = (aabbs[i].upper.x > max) ? aabbs[i].upper.x : max;
	}
}

void dispatchPotentialCollision(Stage& stage, const AABB& a, const AABB& b) {
	// TODO: Switch on type and call appropriate collision routine
	// NOTE: We only check a small subset of the type collisions, anything
	// not included is just undefined and therefore does nothing
	// The only notable exception to this is body collision with seas which is handled per
	// object in their update
	EntityType aType = a.type;
	EntityType bType = b.type;
	switch(aType | bType){
		case ROCK|PLATFORM: break;
		case ROCK|SHIP: break;
		case ROCK|ROCK: break;
		case SHIP|PLATFORM: break;
		default: break;
	}
}


void collide(Rock& rock, Sea& sea) {
}

void collide(Rock& rock, const Platform& platform) {
}

void collide(Rock& rock, Ship& ship) {
}

void collide(Rock& rock, Rock& other_rock) {
}

void collide(Ship& ship, const Sea& sea) {
}

void collide(Ship& ship, const Platform& platform) {
}

void collide(Ship& ship, Rock& rock) {
}

