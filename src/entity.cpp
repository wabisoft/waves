#include <glm/vec2.hpp>

#include "entity.hpp"
#include "aabb.hpp"
#include "stage.hpp"
#include "sea.hpp"
#include "rock.hpp"
#include "ship.hpp"
#include "platform.hpp"

using namespace glm;

EntityHandle getEntityHandle(Stage& stage, Entity entity) {
	switch(entity.type) {
		case NONE:
			assert(false); // this should never happen
			break;
		case SEA: {
				SeaIt sea = findSea(stage, entity.id);
				return {&sea->shape.position, &sea->shape, SEA, sea->id};
			}
			break;
		case PLATFORM: {
				PlatformIt platform = findPlatform(stage, entity.id);
				return {&platform->shape.position, &platform->shape, PLATFORM, platform->id};
			}
			break;
		case ROCK: {
				RockIt rock = findRock(stage, entity.id);
				return {&rock->shape.position, &rock->shape, ROCK, rock->id};
			}
			break;
		case SHIP:
			return {&stage.ship.shape.position, &stage.ship.shape, SHIP, stage.ship.id};
			break;
	}
}

bool pointOnEntity(Stage stage, vec2 point, Entity entity) {
	EntityHandle handle = getEntityHandle(stage, entity);
	return pointInside(point, *handle.pShape);
	// switch(entity.type) {
	// 	case NONE:
	// 		assert(false); // this should never happen
	// 		return false;
	// 		break;
	// 	case SEA:
	// 		{
	// 			Sea& sea = *findSea(stage, entity.id);
	// 			return pointInside(point, sea.shape);
	// 		}
	// 		break;
	// 	case PLATFORM:
	// 		{
	// 			Platform& platform = *findPlatform(stage, entity.id);
	// 			return pointInside(point, platform.shape);
	// 		}
	// 		break;
	// 	case ROCK:
	// 		{
	// 			Rock& rock = *findRock(stage, entity.id);
	// 			return pointInside(point, rock.shape);
	// 		}
	// 		break;
	// 	case SHIP:
	// 		return pointInside(point, stage.ship.shape);
	// 		break;
	// }
	// return false;
}

EntityHandle findEntityAtPosition(Stage& stage, vec2 position) {
	// Adaptation of sweep and prune
	// takes advantage of the sortedness of the two lists used in our collision detection routine
	struct Candidate{
		Axis axis = NO_AXIS;
		Entity entity;
	};
	struct Chosen {
		float squaredDistance;
		EntityHandle handle;
	};
	std::vector<Candidate> candidates;
	for(int a = 0; a < 2; ++a) {
		std::vector<uint8> axisOrder = stage.xAxisOrder;
		Axis axis = X_AXIS;
		if(a>0) {
			axisOrder = stage.yAxisOrder;
			axis = Y_AXIS;
		}
		for(int i = 0; i < axisOrder.size(); ++i) {
			AABB aabb = *findAABB(stage, axisOrder[i]);
			if(aabb.lower[a] <= position[a] && aabb.upper[a] >= position[a]) {
				auto pred = [&aabb](Candidate c) -> bool {
					return c.entity.id == aabb.id;
				};
				auto search = std::find_if(candidates.begin(), candidates.end(), pred);
				if(search != candidates.end()) {
					search->axis = (Axis)(search->axis | axis);
				} else if (a<1) { // if we are on the second axis and the candidate doesn't exist then we can skip
					candidates.push_back({axis, Entity{aabb.id, aabb.type}});
				}
			} else if (aabb.lower[a] >= position[a]) {
				// since the axis list is sorted from lowest to highest then
				// we can say that everything after this is not overlaping
				break;
			}
		}
	}
	Chosen chosen = {std::numeric_limits<float>::infinity(), {}};
	for(Candidate candidate : candidates) {
		if (candidate.axis != BOTH_AXES) { continue; }
		EntityHandle handle = getEntityHandle(stage, candidate.entity);
		vec2 rel = *handle.pPosition - position;
		float squaredDistance = dot(rel, rel);
		if(chosen.squaredDistance > squaredDistance) {
			chosen = {squaredDistance, handle };
		}
	}
	if(chosen.handle.id && pointInside(position, *chosen.handle.pShape)){
		return chosen.handle;
	};

	return {};
}
