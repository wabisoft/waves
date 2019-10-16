#include <cassert>
#include <iostream>
#include <vector>

#include <glm/vec2.hpp>

#include "aabb.hpp"
#include "entity.hpp"
#include "collision.hpp"
#include "constants.hpp"
#include "maths.hpp"
#include "physics.hpp"
#include "platform.hpp"
#include "printing.hpp"
#include "rock.hpp"
#include "shapes.hpp"
#include "stage.hpp"
#include "util.hpp"

using namespace wabi;
using namespace glm;

void resolveCollisions(Stage& stage) {
	// NOTE (owen): if it starts to get slow this is definately a place we can optimize
	if (stage.aabbs.size() == 0) return;
	auto aabbSortPredicate = [](AABB a, AABB b) { return a.lower.x < b.lower.x; };
	// update the AABBs
	updateAABBS(stage);
	// then resort the list (insertion sort is a "slow" sorting method
	// but is very fast in an almost sorted list, which due to spatial coherence
	// this list should be)

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
	insertion_sort<AABB>(stage.xAxisOrder.begin(), stage.xAxisOrder.end(), xSortPredicate);
	insertion_sort<AABB>(stage.yAxisOrder.begin(), stage.yAxisOrder.end(), ySortPredicate);

	std::vector<AABBPair> pairs;
	pairs.reserve(2 * stage.aabbs.size()); // optimistic reserve TODO: cache the pairs
	assert(stage.xAxisOrder.size() == stage.yAxisOrder.size() && stage.xAxisOrder.size() == stage.aabbs.size());
	for (int a = 0; a< 2; ++a) {
		// do the x axis on the fist pass
		Axis axis = X_AXIS;
		std::vector<uint8> axisOrder = stage.xAxisOrder;
		auto check = [&axis](vec2 point) -> float {
			return (axis == X_AXIS) ? point.x : point.y;
		};

		// do the y axis on the second pass
		if (a > 0) {
			axis = Y_AXIS;
			axisOrder = stage.yAxisOrder;
		}
		// start with the first aabb upper as the max
		AABB start = *findAABB(stage, axisOrder[0]);
		float max = check(start.upper);
		// Loop the aabbs and look for collisions
		for (int i = 1; i < axisOrder.size(); ++i) {
			AABB current = *findAABB(stage, axisOrder[i]);
			for (int j = i - 1; j >= 0; --j) {
				AABB previous = *findAABB(stage, axisOrder[j]);
				float prevUp = check(previous.upper);
				float curLow = check(current.lower);
				if (prevUp > curLow) {
				// if (check(previous.upper) > check(current.lower)) {
					// if the pairs list is empty
					if (pairs.empty()) {
						assert(axis == X_AXIS); // this should only happen on the x-axis because of the short circuit at the bottom of the outermost loop
						pairs.push_back({axis, current, previous});
					} else {
						// otherwise
						auto pairPredicate = [&previous, &current](AABBPair& pair) -> bool {
							return (pair.a == previous && pair.b == current) || (pair.a == current && pair.b == previous);
						};
						// look for the pair in the pairs list
						auto search = std::find_if(pairs.begin(), pairs.end(), pairPredicate);
						// if you find it then set the axis
						if (search != pairs.end()) {
							search->setAxis(axis);
						} else if (a<1){ // if this is second axis and we didn't have an overlap already then
							// we don't there's no potential collision
							// otherwise add the pair to the list
							pairs.push_back({axis, current, previous});
						}
					}
				}
				// if the current collider is right of the right most point of our
				// biggest-so-far box then we don't need to keep looking back
				if (check(current.lower) > max) break;
			}
			// if this box's AABB is the biggest we've seen so far then remember it
			max = (check(current.upper) > max) ? check(current.upper) : max;
		}
		if (pairs.empty()) { break; } // if there's no collisions on the first axis then don't bother with the second
	}
	for (AABBPair& pair: pairs) {
		if (collides(pair)) {
			dispatchPotentialCollision(stage, pair);
		}
	}
}

void dispatchPotentialCollision(Stage& stage, const AABBPair& pair) {
	// NOTE: We only check a small subset of the type collisions, anything
	// not included is just undefined and therefore does nothing
	// Get the a entity (if we need to);
	Rock* aRock = nullptr;
	Platform* aPlatform = nullptr;
	Sea * aSea = nullptr;
	// Platform* aPlatform = nullptr;
	switch(pair.a.type) {
		case ROCK:	aRock = &*findRock(stage, pair.a.id); break;
		case PLATFORM:	aPlatform = &*findPlatform(stage, pair.a.id); break;
		case SEA:	aSea= &*findSea(stage, pair.a.id); break;
		default: break;
	}
	Rock* bRock = nullptr;
	Platform* bPlatform = nullptr;
	Sea* bSea = nullptr;
	switch(pair.b.type) {
		case ROCK:	bRock = &*findRock(stage, pair.b.id); break;
		case PLATFORM:	bPlatform = &*findPlatform(stage, pair.b.id); break;
		case SEA:	bSea = &*findSea(stage, pair.b.id); break;
		default: break;
	}
	switch(pair.a.type | pair.b.type){
		case ROCK|SEA:
			if (aRock) { collide(*aRock, *bSea); }
			else if (bRock) { collide(*bRock, *aSea); }
			else { assert(false); }
			break;
		case ROCK|PLATFORM:
			if (aRock && bPlatform) { collide(*aRock, *bPlatform); }
			else if (bRock && aPlatform) { collide(*bRock, *aPlatform); }
			else { assert(false); }
			break;
		case ROCK|SHIP: break;
		case ROCK|ROCK:
			if (aRock && bRock){ collide(*aRock, *bRock); }
			else { assert(false); }
			break;
		case SHIP|SEA:
			if(aSea) { collide(stage.ship, *aSea); }
			else if(bSea) { collide(stage.ship, *bSea); }
			else { assert(false); }
			break;
		case SHIP|PLATFORM:
			if (aPlatform) { collide(stage.ship, *aPlatform); }
			else if (bPlatform) { collide(stage.ship, *bPlatform); }
			else { assert(false); }
			break;
		default: break;
	}
}


inline void collideRed(Rock& rock, Sea& sea) {
	assert(rock.type.type == RockType::RED);
	vec2 rockPosition = rock.shape.position;
	vec2 rockNextFramePosition = rock.shape.position + rock.velocity * FIXED_TIMESTEP;
	vec2 seaUpperBound = upperBound(sea.shape);
	if(rockPosition.y < seaUpperBound.y) {
		createWave(sea, rockPosition, length(rock.velocity) * rock.shape.radius * rock.shape.radius * PI, (short)sign(rock.velocity.x), 1);
		rock.active = 0;
	} else if (rockPosition.y < sea.heightAtX(rockPosition.x)) {
		// TODO: Find nearest wave and maybe distructively interfere with it?
	} else if (rockNextFramePosition.y < seaUpperBound.y) {
		createWave(sea, rockPosition, length(rock.velocity) * rock.shape.radius * rock.shape.radius * PI, (short)sign(rock.velocity.x), 1);
		rock.active = 0;
	}
}

inline void collideGreen(Rock& rock, Sea& sea) {
	assert(rock.type.type == RockType::GREEN);
	float seaHeight = sea.heightAtX(rock.shape.position.x);
	vec2 lower = {rock.shape.position.x, rock.shape.position.y - rock.shape.radius};
	vec2 upper = {rock.shape.position.x, rock.shape.position.y + rock.shape.radius};
	if ( seaHeight < lower.y) {
		// we say this is no collision
		return;
	}
	float displacedWater = area(rock.shape);
	if (upper.y > seaHeight) {
		// float h= seaHeight - lower.y;
		// float r= rock.shape.radius;
		// displacedWater = r * r * std::acos((r-h)/r) * (r-h) * std::sqrt(2*r*h - h*h);
		displacedWater = seaHeight - lower.y;
	}
	rock.velocity += VEC2_UP * displacedWater * GRAVITATIONAL_CONSTANT * FIXED_TIMESTEP;
	rock.velocity += sea.velocityAtX(rock.shape.position.x) * FIXED_TIMESTEP;
	auto drag = dragForce(rock.velocity, 600.f, mass(rock));
	// auto drag = dragForce(rock.velocity, 600.f, area(AABB(rock))*SHIP_AREA_MASS_RATIO);
	rock.velocity += drag * FIXED_TIMESTEP;

	if(rock.state.type != RockState::FLOATING) {
		rock.state = {};
		rock.state.type = RockState::FLOATING;
		rock.state.floating = { 0.f, sea.id};
	}
	if (sea.waves.size() < 0) { return; } // nothing to do for no waves
	WaveIt closestWaveIt = findWaveAtPosition(sea, rock.shape.position);
	if (closestWaveIt == sea.waves.end()) {
		if (sea.waves.size() > 0) assert(false);
		return;
	} // if there are no waves then do nothing
	const Wave& wave = *closestWaveIt;
	float distFromWave = std::abs(rock.shape.position.x - wave.position.x);
	if (distFromWave < 0.1) {
		rock.state = {};
		// rock.state.type = ShipState::SURFING;
		// rock.state.surfing.wave_id = wave.id;
	}
}

inline void collideBlue(Rock& rock, Sea& sea) {
	assert(rock.type.type == RockType::BLUE);
	vec2 rockPosition = rock.shape.position;
	vec2 rockNextFramePosition = rock.shape.position + rock.velocity * FIXED_TIMESTEP;
	vec2 seaUpperBound = upperBound(sea.shape);
	if(rockPosition.y < seaUpperBound.y) {
		createWave(sea, rockPosition, length(rock.velocity) * rock.shape.radius * rock.shape.radius * PI, (short)sign(rock.velocity.x), -1);
		rock.active = 0;
	} else if (rockPosition.y < sea.heightAtX(rockPosition.x)) {
		// TODO: Find nearest wave and maybe distructively interfere with it?
	} else if (rockNextFramePosition.y < seaUpperBound.y) {
		createWave(sea, rockPosition, length(rock.velocity) * rock.shape.radius * rock.shape.radius * PI, (short)sign(rock.velocity.x), -1);
		rock.active = 0;
	}

}

void collide(Rock& rock, Sea& sea) {
	if (!rock.active){
		return;
	}
	switch(rock.type.type) {
		case RockType::RED: return collideRed(rock, sea); break;
		case RockType::GREEN: return collideGreen(rock, sea); break;
		case RockType::BLUE: return collideBlue(rock, sea); break;
	}
}

void collide(Rock& rock, const Platform& platform) {
	// FIXME: still need to figure out tunneling for really fast and small shit // need to make a polygon for the 4 dimensional position of the circle and do something clever with that
	Collision col = collision(rock.shape, platform.shape);
	if (col.collides) {
		if(rock.state.type == RockState::FALLING && col.normal == VEC2_UP) {
			rock.state = {RockState::STANDING, {{col.surfaceStart, col.surfaceEnd}}};
		}
		rock.shape.position += col.normal * col.penetration;
		float j = linearImpulse(rock.velocity, VEC2_ZERO, mass(rock) , mass(platform), ROCK_RESTITUTION);
		rock.velocity += (j/mass(rock)) * col.normal;
	}
}

void collide(Rock& rock, Rock& other_rock) {
	Collision col = collision(rock.shape, other_rock.shape);
	// FIXME: This is makeing some weird behavior, I think it's something to do with the order of collision resolution
	if (col.collides) {
		rock.shape.position += col.normal * col.penetration;
		other_rock.shape.position += -col.normal * col.penetration;
		float j = linearImpulse(rock.velocity, other_rock.velocity, mass(rock), mass(other_rock), ROCK_RESTITUTION);
		rock.velocity += (j/mass(rock)) * col.normal;
		other_rock.velocity += (j/mass(other_rock)) * -col.normal;
	}
}

void collide(Rock& rock, Ship& ship) {
}

void collide(Ship& ship, Sea& sea) {
	float seaHeight = sea.heightAtX(ship.shape.position.x);
	vec2 lower, upper;
	boundingPoints(ship.shape, lower, upper);
	if ( seaHeight < lower.y) {
		// we say this is no collision
		return;
	}
	float displacedWater = area((const Rectangle&)ship.shape);
	if (upper.y > seaHeight) {
		displacedWater = (seaHeight - lower.y);
	}
	ship.velocity += VEC2_UP * displacedWater * GRAVITATIONAL_CONSTANT * FIXED_TIMESTEP;
	ship.velocity += sea.velocityAtX(ship.shape.position.x) * FIXED_TIMESTEP;
	auto drag = dragForce(ship.velocity, 600.f, mass(ship) * SHIP_AREA_MASS_RATIO);
	ship.velocity += drag * FIXED_TIMESTEP;

	if (sea.waves.size() < 0) { return; } // nothing to do for no waves
	WaveIt closestWaveIt = findWaveAtPosition(sea, ship.shape.position);
	if (closestWaveIt == sea.waves.end()) {
		if (sea.waves.size() > 0) assert(false);
		return;
	} // if there are no waves then do nothing
	const Wave& wave = *closestWaveIt;
	float distFromWave = std::abs(ship.shape.position.x - wave.position.x);
	if (distFromWave < 0.3) {
		ship.state = {};
		ship.state.type = ShipState::SURFING;
		ship.state.surfing.waveId = wave.id;
		ship.state.surfing.seaId = sea.id;
		ship.state.surfing.waveDirection = wave.direction;
	}
}

void collide(Ship& ship, const Platform& platform) {
	Collision col = collision(ship.shape, platform.shape);
	if(col.collides) {
		if(ship.state.type != ShipState::STANDING) {
			if (col.normal == VEC2_UP) {
				ship.state = {ShipState::STANDING, {{col.surfaceStart, col.surfaceEnd}}};
			} else {
				ship.state = {ShipState::FALLING, {}};
			}
		}
		ship.shape.position += col.normal * col.penetration;
		float j = linearImpulse(ship.velocity, VEC2_ZERO, mass(ship), mass(platform), SHIP_RESTITUTION);
		ship.velocity += (j/mass(ship)) * col.normal;
	}
}

void collide(Ship& ship, Rock& rock) {
}


Collision collision(const Circle& c1, const Circle& c2) {
	vec2 relativePosition = c1.position - c2.position;
	float sumOfRadii = (c1.radius + c2.radius);
	float sumOfRadiiSquared =  sumOfRadii * (c1.radius + c2.radius);
	if (dot(relativePosition, relativePosition) < sumOfRadiiSquared) {
		vec2 normalizedRelPos = normalize(relativePosition); // normalize in place;
		Collision col;
		col.collides = true;
		col.intersection = c1.position + c1.radius * normalizedRelPos;
		// col.normal = normalized(col.intersection - c2.position);
		col.normal = normalize(c1.position - c2.position);
		col.penetration = std::abs(length(relativePosition) - sumOfRadii);
		return col;
	} else {
		return Collision();
	}
}
