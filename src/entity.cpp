#include <glm/vec2.hpp>

#include "entity.hpp"
#include "aabb.hpp"
#include "collision.hpp"
#include "stage.hpp"
#include "sea.hpp"
#include "rock.hpp"
#include "ship.hpp"
#include "platform.hpp"

using namespace glm;


Entity* getEntity(Stage& stage, uint8 id, Entity::Type type) {
	switch(type) {
		case Entity::NONE:
			assert(false); // this should never happen
			break;
		case Entity::SEA: {
				return &(*findSea(stage, id));
			}
			break;
		case Entity::PLATFORM: {
				return &(*findPlatform(stage, id));
			}
			break;
		case Entity::ROCK: {
				return &(*findRock(stage, id));
			}
			break;
		case Entity::SHIP:
			return &stage.ship;
			break;
		case Entity::WIN:
			return &stage.win;
			break;
	}
}

// bool pointOnEntity(Stage stage, vec2 point, Entity entity) {
// 	EntityHandle handle = getEntityHandle(stage, entity);
// 	return pointInside(point, *handle.pShape);
// }

Entity* findEntityAtPosition(Stage& stage, vec2 position) {
	// Adaptation of sweep and prune
	// takes advantage of the sortedness of the two lists used in our collision detection routine
	updateBroadPhase(stage);
	float bump = 0.5f;
	struct Candidate{
		Axis axis = NO_AXIS;
		Entity* pEntity;
	};
	struct Chosen {
		float squaredDistance;
		Entity* pEntity;
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
			if(aabb.lower[a] - bump <= position[a] && aabb.upper[a]+bump >= position[a]) {
				auto pred = [&aabb](Candidate c) -> bool {
					return c.pEntity->id == aabb.id;
				};
				auto search = std::find_if(candidates.begin(), candidates.end(), pred);
				if(search != candidates.end()) {
					search->axis = (Axis)(search->axis | axis);
				} else if (a<1) { // if we are on the second axis and the candidate doesn't exist then we can skip
					candidates.push_back({axis, getEntity(stage, aabb.id, aabb.type)});
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
		vec2 rel = candidate.pEntity->position - position;
		float squaredDistance = dot(rel, rel);
		if(chosen.squaredDistance > squaredDistance) {
			chosen = {squaredDistance, candidate.pEntity};
		}
	}
	if(chosen.pEntity && pointInside(position, chosen.pEntity->shape)){
		return chosen.pEntity;
	};

	return nullptr;
}
