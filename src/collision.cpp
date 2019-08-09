#include <intrin.h>
#include <cassert>
#include <iostream>
#include <vector>

#include "entity.hpp"
#include "collision.hpp"
#include "util.hpp"

void resolveCollisions(Stage& stage) {
	// NOTE (owen): if it starts to get slow this is definately a place we can optimize
	//
	// OPTMZ : keep aabbs attached to stage struct and manage it with the lifetime of other objects?

	AABB aabbs[MAX_AABBS];
	size_t numAABBs = 0;
	auto aabbSortPredicate = [](AABB a, AABB b) { return a.lower.x < b.lower.x; };
	for (size_t i = 0; i < stage.numRocks; ++i) {
		sorted_insert(aabb(stage.rocks[i]), aabbs, numAABBs, aabbSortPredicate);
	}
	for(size_t i = 0; i < stage.numPlatforms; ++i){
		sorted_insert(aabb(stage.platforms[i]), aabbs, numAABBs, aabbSortPredicate);
	}
	//	sorted_insert(aabb(stage.ship), aabbs, numAABBs, aabbSortPredicate);
	if (numAABBs == 0) return;
	
	std::vector<Collision> collisions;
	collisions.reserve(2*MAX_AABBS); // In a worst case scenario we could really have like MAX_ABBS^2 collisions but let's be optimistic

	AABB seaAAABB = aabb(stage.sea);
	// start with the first aabb upper as the max
	float max = aabbs[0].upper.x;
	// Loop the aabbs and look for collisions
	collisions.push_back({aabbs[0], seaAAABB}); // just check everyone for collision with sea
	for (size_t i = 1; i < numAABBs; ++i){
		collisions.push_back({aabbs[i], seaAAABB}); // just check everyone for collision with sea
		for (size_t j = i-1; j > 0; --j) {
			if (aabbs[j].upper.x > aabbs[i].lower.x) {
				// OPTMZ: maybe push to a y array in sorted order and then run the sweep on that array again? (optimization)
				collisions.push_back({aabbs[i], aabbs[j]});
			}
			// if the current collider is right of the right most point of our
			// biggest-so-far box then we don't need to keep looking bac
			if (aabbs[i].lower.x > max) break;
		}
		// if this box's rect is the biggest we've seen so far then remember it
		max = (aabbs[i].upper.x > max) ? aabbs[i].upper.x : max;
	}
	for (size_t i = 0; i < collisions.size(); ++i) {
		dispatchPotentialCollision(stage, collisions[i]);
	}
}

void dispatchPotentialCollision(Stage& stage, const Collision& collision) {
	// TODO: Switch on type and call appropriate collision routine
	// NOTE: We only check a small subset of the type collisions, anything
	// not included is just undefined and therefore does nothing

	EntityType aType = collision.a.type;
	EntityType bType = collision.b.type;
	switch(aType | bType){
		case ROCK|SEA:
			switch(aType) {
				case ROCK: collide(*(Rock*)collision.a.entity, stage.sea); break;
				case SEA: collide(*(Rock*)collision.b.entity, stage.sea); break;
				default: break;
			}; break;
		case ROCK|PLATFORM: 
			switch(aType) {
				case ROCK: collide(*(Rock*)collision.a.entity, *(Platform*)collision.b.entity); break;
				case PLATFORM: collide(*(Rock*)collision.b.entity, *(Platform*)collision.a.entity); break;
				default: break;
			} break;
		case ROCK|SHIP:
			switch(aType) {
				case ROCK: collide(*(Rock*)collision.a.entity, *(Ship*)collision.b.entity); break;
				case SHIP: collide(*(Rock*)collision.b.entity, *(Ship*)collision.a.entity); break;
				default: break;
			} break;
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

