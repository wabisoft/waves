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

void updateBroadPhase(Stage& stage) {
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
}

void resolveCollisions(Stage& stage) {
	// NOTE (owen): if it starts to get slow this is definately a place we can optimize
	if (stage.aabbs.size() == 0) return;
	updateBroadPhase(stage);

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
		case Entity::ROCK:	aRock = &*findRock(stage, pair.a.id); break;
		case Entity::PLATFORM:	aPlatform = &*findPlatform(stage, pair.a.id); break;
		case Entity::SEA:	aSea= &*findSea(stage, pair.a.id); break;
		default: break;
	}
	Rock* bRock = nullptr;
	Platform* bPlatform = nullptr;
	Sea* bSea = nullptr;
	switch(pair.b.type) {
		case Entity::ROCK:	bRock = &*findRock(stage, pair.b.id); break;
		case Entity::PLATFORM:	bPlatform = &*findPlatform(stage, pair.b.id); break;
		case Entity::SEA:	bSea = &*findSea(stage, pair.b.id); break;
		default: break;
	}
	switch(pair.a.type | pair.b.type){
		case Entity::ROCK|Entity::SEA:
			if (aRock) { collide(*aRock, *bSea); }
			else if (bRock) { collide(*bRock, *aSea); }
			else { assert(false); }
			break;
		case Entity::ROCK|Entity::PLATFORM:
			if (aRock && bPlatform) { collide(*aRock, *bPlatform); }
			else if (bRock && aPlatform) { collide(*bRock, *aPlatform); }
			else { assert(false); }
			break;
		case Entity::ROCK|Entity::SHIP: break;
		case Entity::ROCK|Entity::ROCK:
			if (aRock && bRock){ collide(*aRock, *bRock); }
			else { assert(false); }
			break;
		case Entity::SHIP|Entity::SEA:
			if(aSea) { collide(stage.ship, *aSea); }
			else if(bSea) { collide(stage.ship, *bSea); }
			else { assert(false); }
			break;
		case Entity::SHIP|Entity::PLATFORM:
			if (aPlatform) { collide(stage.ship, *aPlatform); }
			else if (bPlatform) { collide(stage.ship, *bPlatform); }
			else { assert(false); }
			break;
		case Entity::SHIP|Entity::WIN:
			collide(stage.ship, stage.win);
		default: break;
	}
}


inline void collideRed(Rock& rock, Sea& sea) {
	assert(rock.kind == Rock::RED);
	if (rock.state.type == Rock::State::SINKING) {
		return;
	}
	vec2 rockPosition = rock.position;
	vec2 rockNextFramePosition = rock.position + rock.velocity * FIXED_TIMESTEP;
	vec2 seaUpperBound = upperBound(sea.shape);
	if(rockPosition.y < seaUpperBound.y) {
		createWave(sea, rockPosition, length(rock.velocity) * rock.radius * rock.radius * PI, (short)sign(rock.velocity.x), 1);
		rock.state.type = Rock::State::SINKING;
		rock.state.sinking.time = 0;
		rock.velocity *= 0.5f;
	} else if (rockPosition.y < sea.heightAtX(rockPosition.x)) {
		// TODO: Find nearest wave and maybe distructively interfere with it?
	} else if (rockNextFramePosition.y < seaUpperBound.y) {
		createWave(sea, rockPosition, length(rock.velocity) * rock.radius * rock.radius * PI, (short)sign(rock.velocity.x), 1);
		rock.state.type = Rock::State::SINKING;
		rock.state.sinking.time = 0;
		rock.velocity *= 0.5f;
	}
}

inline void collideGreen(Rock& rock, Sea& sea) {
	assert(rock.kind == Rock::GREEN);
	float seaHeight = sea.heightAtX(rock.position.x);
	vec2 lower = {rock.position.x, rock.position.y - rock.radius};
	vec2 upper = {rock.position.x, rock.position.y + rock.radius};
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
	rock.velocity += sea.velocityAtX(rock.position.x) * FIXED_TIMESTEP;
	auto drag = dragForce(rock.velocity, 600.f, mass(rock));
	// auto drag = dragForce(rock.velocity, 600.f, area(AABB(rock))*SHIP_AREA_MASS_RATIO);
	rock.velocity += drag * FIXED_TIMESTEP;

	if(rock.state.type != Rock::State::FLOATING) {
		rock.state.type = Rock::State::FLOATING;
		rock.state.floating = { 0.f, sea.id};
	}
	if (sea.waves.size() < 0) { return; } // nothing to do for no waves
	WaveIt closestWaveIt = findWaveAtPosition(sea, rock.position);
	if (closestWaveIt == sea.waves.end()) {
		if (sea.waves.size() > 0) assert(false);
		return;
	} // if there are no waves then do nothing
	const Wave& wave = *closestWaveIt;
	float distFromWave = std::abs(rock.position.x - wave.position.x);
	if (distFromWave < 0.1) {
		rock.state.type = Rock::State::FALLING;
		rock.state.falling = {};
	}
}

inline void collideBlue(Rock& rock, Sea& sea) {
	assert(rock.kind == Rock::BLUE);
	if (rock.state.type == Rock::State::SINKING) {
		return;
	}
	vec2 rockPosition = rock.position;
	vec2 rockNextFramePosition = rock.position + rock.velocity * FIXED_TIMESTEP;
	vec2 seaUpperBound = upperBound(sea.shape);
	if(rockPosition.y < seaUpperBound.y) {
		createWave(sea, rockPosition, length(rock.velocity) * rock.radius * rock.radius * PI, (short)sign(rock.velocity.x), -1);
		rock.state.type = Rock::State::SINKING;
		rock.state.sinking.time = 0;
		rock.velocity *= 0.5f;

	} else if (rockPosition.y < sea.heightAtX(rockPosition.x)) {
		// TODO: Find nearest wave and maybe distructively interfere with it?
	} else if (rockNextFramePosition.y < seaUpperBound.y) {
		createWave(sea, rockPosition, length(rock.velocity) * rock.radius * rock.radius * PI, (short)sign(rock.velocity.x), -1);
		rock.state.type = Rock::State::SINKING;
		rock.state.sinking.time = 0;
		rock.velocity *= 0.5f;
	}
}

void collide(Rock& rock, Sea& sea) {
	switch(rock.kind) {
		case Rock::RED: return collideRed(rock, sea); break;
		case Rock::GREEN: return collideGreen(rock, sea); break;
		case Rock::BLUE: return collideBlue(rock, sea); break;
	}
}

void collide(Rock& rock, const Platform& platform) {
	// FIXME: still need to figure out tunneling for really fast and small shit // need to make a polygon for the 4 dimensional position of the circle and do something clever with that
	Collision col = collision(rock.position, platform.position, rock.shape, platform.shape);
	if (col.collides) {
		if(rock.state.type == Rock::State::FALLING && col.normal == VEC2_UP) {
			rock.state.type = Rock::State::STANDING;
			rock.state.standing = {col.surfaceStart, col.surfaceEnd};
		}
		rock.position += col.normal * col.penetration;
		float j = linearImpulse(rock.velocity, VEC2_ZERO, mass(rock) , mass(platform), ROCK_RESTITUTION);
		rock.velocity += (j/mass(rock)) * col.normal;
	}
}

void collide(Rock& rock, Rock& other_rock) {
	Collision col = collision(rock.position, other_rock.position, rock.shape, other_rock.shape);
	// FIXME: This is makeing some weird behavior, I think it's something to do with the order of collision resolution
	if (col.collides) {
		rock.position += col.normal * col.penetration;
		other_rock.position += -col.normal * col.penetration;
		float j = linearImpulse(rock.velocity, other_rock.velocity, mass(rock), mass(other_rock), ROCK_RESTITUTION);
		rock.velocity += (j/mass(rock)) * col.normal;
		other_rock.velocity += (j/mass(other_rock)) * -col.normal;
	}
}

void collide(Rock& rock, Ship& ship) {
}

void collide(Ship& ship, Sea& sea) {
	Polygon c = clip(ship.shape, sea.shape);
	float displacedWater = area(c);
	if (displacedWater == 0) {
		return;
	}
	ship.velocity += VEC2_UP * displacedWater * GRAVITATIONAL_CONSTANT * FIXED_TIMESTEP;
	ship.velocity += sea.velocityAtX(ship.position.x) * FIXED_TIMESTEP;
	//auto drag = dragForce(ship.velocity, WATER_DENSITY, mass(ship));
	auto drag = dragForce(ship.velocity, WATER_DENSITY, WATER_DENSITY*displacedWater);
	ship.velocity += drag * FIXED_TIMESTEP;

	if (sea.waves.size() < 0) { return; } // nothing to do for no waves
	WaveIt closestWaveIt = findWaveAtPosition(sea, ship.position);
	if (closestWaveIt == sea.waves.end()) {
		if (sea.waves.size() > 0) assert(false);
		return;
	} // if there are no waves then do nothing
	const Wave& wave = *closestWaveIt;
	float distFromWave = std::abs(ship.position.x - wave.position.x);
	if (distFromWave < 0.3) {
		ship.state.type = Ship::State::SURFING;
		ship.state.surfing.waveId = wave.id;
		ship.state.surfing.seaId = sea.id;
		ship.state.surfing.waveDirection = wave.direction;
	}
}

void collide(Ship& ship, const Platform& platform) {
	Collision col = collision(ship.position, platform.position, ship.shape, platform.shape);
	if(col.collides) {
		if(ship.state.type != Ship::State::STANDING) {
			if (col.normal == VEC2_UP) {
				ship.state.type = Ship::State::STANDING;
				ship.state.standing = {col.surfaceStart, col.surfaceEnd};
			} else {
				ship.state = {Ship::State::FALLING, {}};
			}
		}
		ship.position += col.normal * col.penetration;
		float j = linearImpulse(ship.velocity, VEC2_ZERO, mass(ship), mass(platform), SHIP_RESTITUTION);
		float adjusted_j = j/mass(ship);
		ship.velocity += adjusted_j * col.normal;
		// vec2 contact = (col.surfaceStart + col.surfaceEnd) / 2.f;
		// vec2 relpos = contact - ship.position;
		// float l = length(vectorRejection(relpos, col.normal));
		// float b = length(vectorProjection(relpos, col.normal));
		// vec2 dims = ship.shape.model[1] - ship.shape.model[3];
		// float h = dims.y;
		// float w = dims.x;
		// float alpha_numerator = (GRAVITATIONAL_CONSTANT * l);
		// float alpha_denominator = (1/12 * (h*h + w*w) + std::sqrt(l*l + b*b));
		// float alpha = alpha_numerator / alpha_denominator;
		// ship.alpha += alpha;
	}
}

void collide(Ship& ship, Rock& rock) {
}

void collide(Ship& ship, Win& win) {
	Polygon c = clip(ship.shape, win.shape);
	float shipArea = area(ship.shape);
	float clipArea = area(c);
	if(clipArea/shipArea > 0.8) {
		ship.inWin = true;
	} else {
		ship.inWin = false;
	}
}

// Thanks javidx9!
// NOTE: this only determines if poly1 is specifically overlaping poly2
// Also NOTE: poly1 and poly2 must be CONVEX
Collision collision(const vec2 p1Pos, const vec2 p2Pos, const Polygon& p1, const Polygon& p2) {
	// check the diagonals of one polygon ...
	std::vector<Collision> collisions;
	for(int a = 0; a < 2; a++) {
		Polygon poly1 = p1;
		Polygon poly2 = p2;
		vec2 poly1Pos = p1Pos;
		if(a > 0) {
			poly1 = p2;
			poly2 = p1;
			vec2 poly1Pos = p2Pos;
		}
		for (int i = 0; i < poly1.size; ++i) {
			glm::vec2 a = poly1Pos;
			glm::vec2 b = poly1.vertices[i];
			// ...against the edges of the other
			for (int j = 0; j < poly2.size; ++j) {
				glm::vec2 c = poly2.vertices[j];
				glm::vec2 d = poly2.vertices[(j+1) % poly2.size];
				Collision col;
				if(lineSegmentIntersection(a,b,c,d,col.intersection)) {
					col.collides = true;
					col.penetration = glm::length(b-col.intersection);
					col.normal = glm::normalize(findNormal(c, d, a));
					col.surfaceStart = c;
					col.surfaceEnd = d;
					collisions.push_back(col);
				}
			}
		}
		if (!collisions.empty()) {
			Collision col = collisions[0];
			glm::vec2 rel = poly1Pos - col.intersection;
			float min = glm::dot(rel, rel);
			for(int i = 1; i < collisions.size(); ++i) {
				glm::vec2 rel_i = poly1Pos - collisions[i].intersection;
				float sqrDst = glm::dot(rel_i, rel_i);
				if (sqrDst < min) {
					col = collisions[i];
					min = sqrDst;
				}
			}
			return col;
		}
	}
	return Collision();
}

