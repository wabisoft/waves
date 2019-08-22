#include <cassert>
#include <iostream>
#include <vector>

#include "aabb.hpp"
#include "entity.hpp"
#include "collision.hpp"
#include "constants.hpp"
#include "physics.hpp"
#include "platform.hpp"
#include "printing.hpp"
#include "rock.hpp"
#include "stage.hpp"
#include "util.hpp"

void resolveCollisions(Stage& stage) {
	// NOTE (owen): if it starts to get slow this is definately a place we can optimize
	int& numAABBs = stage.numAABBS;
	if (numAABBs == 0) return;
	AABB* aabbs = stage.aabbs;
	auto aabbSortPredicate = [](AABB a, AABB b) { return a.lower.x < b.lower.x; };
	for(int i = 0; i < numAABBs; ++i) {
		switch (aabbs[i].type) {
		case ROCK:
			{
				int rock_idx = binary_find_where(aabbs[i].id, stage.rocks, stage.numRocks, [](const Rock& rock){ return rock.id;});
				assert(rock_idx > -1);
				aabbs[i] = AABB(stage.rocks[rock_idx]);
			}
		break;
		case SHIP:
			aabbs[i] = AABB(stage.ship);
		break;
		case PLATFORM:
			// NOTE: since platforms are static, their AABBS shouldn't need updating, I'll leave the case here in case
		break;
		default:
		break;
		}
	}
	// then resort the list (insertion sort is a "slow" sorting method
	// but is very fast in an almost sorted list, which due to spatial coherence
	// this list should be)
	insertion_sort(aabbs, numAABBs, aabbSortPredicate);

	std::vector<AABBPair> pairs;
	pairs.reserve(2*MAX_AABBS); // In a worst case scenario we could really have like MAX_ABBS^2 collisions but let's be optimistic

	AABB seaAAABB = AABB(stage.sea); // because the sea AABB spans the entire stage, we don't want to include it in the sweep list as it will slow down the algorithm
	// start with the first aabb upper as the max
	float max = aabbs[0].upper.x;
	// Loop the aabbs and look for collisions
	pairs.push_back({aabbs[0], seaAAABB}); // just check everyone for collision with sea
	for (int i = 1; i < numAABBs; ++i){
		pairs.push_back({aabbs[i], seaAAABB}); // just check everyone for collision with sea
		for (int j = i-1; j >= 0; --j) {
			if (aabbs[j].upper.x > aabbs[i].lower.x) {
				// OPTMZ: sweep y-axis too
				pairs.push_back({aabbs[i], aabbs[j]});
			}
			// if the current collider is right of the right most point of our
			// biggest-so-far box then we don't need to keep looking back
			if (aabbs[i].lower.x > max) break;
		}
		// if this box's AABB is the biggest we've seen so far then remember it
		max = (aabbs[i].upper.x > max) ? aabbs[i].upper.x : max;
	}
	for (int i = 0; i < pairs.size(); ++i) {
		dispatchPotentialCollision(stage, pairs[i]);
	}
}

void dispatchPotentialCollision(Stage& stage, const AABBPair& pair) {
	// NOTE: We only check a small subset of the type collisions, anything
	// not included is just undefined and therefore does nothing
	// Get the a entity (if we need to);
	Rock* aRock = nullptr;
	Platform* aPlatform = nullptr;
	// Platform* aPlatform = nullptr;
	switch(pair.a.type) {
		case ROCK:	aRock = &findRock(stage, pair.a.id); break;
		case PLATFORM:	aPlatform = &findPlatform(stage, pair.a.id); break;
		default: break;
	}
	Rock* bRock = nullptr;
	Platform* bPlatform= nullptr;
	switch(pair.b.type) {
		case ROCK:	bRock = &findRock(stage, pair.b.id); break;
		case PLATFORM:	bPlatform = &findPlatform(stage, pair.b.id); break;
		default: break;
	}
	switch(pair.a.type | pair.b.type){
		case ROCK|SEA:
			if (aRock) collide(*aRock, stage.sea);
			else if (bRock) collide(*bRock, stage.sea);
			else assert(false);
			break;
		case ROCK|PLATFORM:
			if (aRock && bPlatform) collide(*aRock, *bPlatform);
			else if (bRock && aPlatform) collide(*bRock, *aPlatform);
			else assert(false);
			break;
		case ROCK|SHIP: break;
		case ROCK|ROCK:
			if (aRock && bRock){ collide(*aRock, *bRock); }
			else { assert(false); }
			break;
		case SHIP|SEA: break;
		case SHIP|PLATFORM: break;
		default: break;
	}
}


void collide(Rock& rock, Sea& sea) {
	if (!rock.active){
		return;
	}
	Vector2 rockPosition = rock.shape.position;
	Vector2 rockNextFramePosition = rock.shape.position + rock.velocity * FIXED_TIMESTEP;
	if(rockPosition.y < sea.level) {
		createWave(sea, rockPosition, magnitude(rock.velocity) * rock.shape.radius * rock.shape.radius * PI);
		rock.active = 0;
	} else if (rockPosition.y < heightAtX(sea, rockPosition.x)) {
		// TODO: Find nearest wave and maybe distructively interfere with it?
	} else if (rockNextFramePosition.y < sea.level) {
		createWave(sea, rockPosition, magnitude(rock.velocity) * rock.shape.radius * rock.shape.radius * PI);
		rock.active = 0;
	}
}

void collide(Rock& rock, const Platform& platform) {
	// FIXME: still need to figure out tunneling for really fast shit // need to make a polygon for the 4 dimensional position of the circle and do something clever with that
	Collision col = collision(rock.shape, platform.shape);
	if (col.collides) {
		if(rock.state.type == RockState::FALLING && col.normal == VECTOR2_UP) {
			rock.state = {RockState::STANDING, {{col.surfaceStart, col.surfaceEnd}}};
		}
		rock.shape.position += col.normal * col.penetration;
		float j = linearImpulse(rock.velocity, VECTOR2_ZERO, mass(rock) , mass(platform), ROCK_RESTITUTION);
		rock.velocity += (j/mass(rock)) * col.normal;
	}
}

void collide(Rock& rock, Rock& other_rock) {
	Collision col = collision(rock.shape, other_rock.shape);
	if (col.collides) {
		rock.shape.position += col.normal * 0.5f * col.penetration;
		other_rock.shape.position += -col.normal * 0.5f * col.penetration;
		rock.velocity += linearImpulse(rock.velocity, other_rock.velocity, rock.shape.radius * ROCK_RADIUS_MASS_RATIO, other_rock.shape.radius * ROCK_RADIUS_MASS_RATIO, ROCK_RESTITUTION) * col.normal;
		other_rock.velocity += linearImpulse(other_rock.velocity, other_rock.velocity, other_rock.shape.radius * ROCK_RADIUS_MASS_RATIO, rock.shape.radius * ROCK_RADIUS_MASS_RATIO, ROCK_RESTITUTION) * -1 * col.normal;
	}
}

void collide(Rock& rock, Ship& ship) {
}

void collide(Ship& ship, const Sea& sea) {
}

void collide(Ship& ship, const Platform& platform) {
}

void collide(Ship& ship, Rock& rock) {
}


Collision collision(const Circle& c1, const Circle& c2) {
	Vector2 relativePosition = c1.position - c2.position;
	float sumOfRadii = (c1.radius + c2.radius);
	float sumOfRadiiSquared =  sumOfRadii * (c1.radius + c2.radius);
	if (squaredMagnitude(relativePosition) < sumOfRadiiSquared) {
		Vector2 normalizedRelPos = normalized(relativePosition); // normalize in place;
		Collision col;
		col.collides = true;
		col.intersection = c1.position + c1.radius * normalizedRelPos;
		col.normal = normalized(col.intersection - c2.position);
		col.penetration = std::abs(magnitude(relativePosition) - sumOfRadii);
		return col;
	} else {
		return Collision();
	}
}
