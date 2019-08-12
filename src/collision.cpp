// #ifdef _WIN32 
// #include <intrin.h>
// Do we need this?
// #endif
#include <cassert>
#include <iostream>
#include <vector>

#include "entity.hpp"
#include "collision.hpp"
#include "constants.hpp"
#include "rock.hpp"
#include "util.hpp"

void resolveCollisions(Stage& stage) {
	// NOTE (owen): if it starts to get slow this is definately a place we can optimize
	// OPTMZ : keep aabbs attached to stage struct and manage it with the lifetime of other objects?
	size_t& numAABBs = stage.numAABBS;
	if (numAABBs == 0) return;
	AABB* aabbs = stage.aabbs;
	auto aabbSortPredicate = [](AABB a, AABB b) { return a.lower.x < b.lower.x; };
	for(size_t i = 0; i < numAABBs; ++i) {
		// TODO: update aabbs with their callback;
		// FIXME: the pointers of aabb.entity are all missaligned. and this is wreaking havoc here, only happens
		// after first aabb deletion(so copy/move semantics are messy? Look into it)
		switch (aabbs[i].type) {
		case ROCK:
			{
				size_t rock_idx = binary_find_where(aabbs[i].id, stage.rocks, stage.numRocks, [](const Rock& rock){ return rock.id;});
				assert(rock_idx < stage.numRocks);
				aabbs[i] = AABB(stage.rocks[rock_idx]);
			}
		break;
		case SHIP:
			aabbs[i] = AABB(stage.ship);
		break;
		case PLATFORM:
			// NOTE: since platforms are static, their AABBS should need updating, I'll leave here in case
		break;
		default:
		break;
		}
		// aabbs[i].updateCallback(aabbs[i]);
	}
	// then resort the list (insertion sort is a "slow" sorting method
	// but is very fast in an almost sorted list, which due to spatial coherence
	// this list should be)
	insertion_sort(aabbs, numAABBs, aabbSortPredicate);

	std::vector<Collision> collisions;
	collisions.reserve(2*MAX_AABBS); // In a worst case scenario we could really have like MAX_ABBS^2 collisions but let's be optimistic

	AABB seaAAABB = AABB(stage.sea);
	// start with the first aabb upper as the max
	float max = aabbs[0].upper.x;
	// Loop the aabbs and look for collisions
	collisions.push_back({aabbs[0], seaAAABB}); // just check everyone for collision with sea
	for (size_t i = 1; i < numAABBs; ++i){
		collisions.push_back({aabbs[i], seaAAABB}); // just check everyone for collision with sea
		for (size_t j = i-1; j > 0; --j) {
			if (aabbs[j].upper.x > aabbs[i].lower.x) {
				// OPTMZ: sweep y-axis too
				collisions.push_back({aabbs[i], aabbs[j]});
			}
			// if the current collider is right of the right most point of our
			// biggest-so-far box then we don't need to keep looking back
			if (aabbs[i].lower.x > max) break;
		}
		// if this box's AABB is the biggest we've seen so far then remember it
		max = (aabbs[i].upper.x > max) ? aabbs[i].upper.x : max;
	}
	for (size_t i = 0; i < collisions.size(); ++i) {
		dispatchPotentialCollision(stage, collisions[i]);
	}
}

void dispatchPotentialCollision(Stage& stage, const Collision& collision) {
	// NOTE: We only check a small subset of the type collisions, anything
	// not included is just undefined and therefore does nothing
	// Get the a entity (if we need to);
	Rock* aRock = nullptr;
	// Platform* aPlatform = nullptr;
	switch(collision.a.type) {
		case ROCK:	aRock = &findRock(stage, collision.a.id); break;
		default: break;
	}
	Rock* bRock = nullptr;
	switch(collision.b.type) {
		case ROCK:	bRock = &findRock(stage, collision.b.id); break;
		default: break;
	}
	switch(collision.a.type| collision.b.type){
		case ROCK|SEA:
			if (aRock) collide(*aRock, stage.sea);
			else collide(*bRock, stage.sea);
			break;
		case ROCK|PLATFORM: break;
		case ROCK|SHIP: break;
		case ROCK|ROCK: break;
		case SHIP|SEA: break;
		case SHIP|PLATFORM: break;
		default: break;
	}
}


void collide(Rock& rock, Sea& sea) {
	if (!rock.active){
		return;
	}
	Vector2 rockPosition = rock.position;
	Vector2 rockNextFramePosition = rock.position + rock.velocity * FIXED_TIMESTEP;
	if(rockPosition.y < sea.level) {
		// collision detection with sea is very easy on a per rock basis, and only complicates the collision detection broad phase if done there.
		// so instead we do it here.
		createWave(sea, rockPosition, magnitude(rock.velocity) * rock.radius * rock.radius * PI);
		rock.active = 0;
	} else if (rockPosition.y < heightAtX(sea, rockPosition.x)) {
		// TODO: Find nearest wave and maybe distructively interfere with it?
	} else if (rockNextFramePosition.y < sea.level) {
		createWave(sea, rockPosition, magnitude(rock.velocity) * rock.radius * rock.radius * PI);
		rock.active = 0;
	}
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

