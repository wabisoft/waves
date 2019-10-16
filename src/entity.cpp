#include <glm/vec2.hpp>

#include "entity.hpp"
#include "aabb.hpp"
#include "stage.hpp"
#include "sea.hpp"
#include "rock.hpp"
#include "ship.hpp"
#include "platform.hpp"

using namespace glm;

vec2 getEntityPosition(Stage& stage,Entity entity) {
	switch(entity.type) {
		case NONE:
			assert(false); // this should never happen
			return VEC2_ZERO;
			break;
		case SEA:
			return findSea(stage, entity.id)->shape.position;
			break;
		case PLATFORM:
			return findPlatform(stage, entity.id)->shape.position;
			break;
		case ROCK:
			return findRock(stage, entity.id)->shape.position;
			break;
		case SHIP:
			return stage.ship.shape.position;
			break;
	}
}

bool pointOnEntity(Stage stage, vec2 point, Entity entity) {
	switch(entity.type) {
		case NONE:
			assert(false); // this should never happen
			return false;
			break;
		case SEA:
			{
				Sea& sea = *findSea(stage, entity.id);
				return pointInsidePolygon(point, sea.shape);
			}
			break;
		case PLATFORM:
			{
				Platform& platform = *findPlatform(stage, entity.id);
				return pointInsidePolygon(point, platform.shape);
			}
			break;
		case ROCK:
			{
				Rock& rock = *findRock(stage, entity.id);
				return pointInsideCircle(point, rock.shape);
			}
			break;
		case SHIP:
			return pointInsidePolygon(point, stage.ship.shape);
			break;
	}

}

Entity findEntityAtPosition(Stage& stage, vec2 position) {
	// Adaptation of sweep and prune
	// takes advantage of the sortedness of the two lists used in our collision detection routine
	struct Candidate{
		Axis axis = NO_AXIS;
		Entity entity;
	};
	struct Chosen {
		float squaredDistance;
		Entity entity;
	};
	std::vector<Candidate> candidates;
	Chosen chosen = {std::numeric_limits<float>::infinity(), {0, NONE}};
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
					// candidates.push_back({axis, Entity{aabb.id, aabb.type}});
					Entity entity = {aabb.id, aabb.type};
					vec2 entityPosition = getEntityPosition(stage, entity);
					vec2 rel = entityPosition - position;
					float squaredDistance = dot(rel, rel);
					if(chosen.squaredDistance > squaredDistance) {
						chosen = {squaredDistance, entity};
					}
				}
			} else if (aabb.lower[a] >= position[a]) {
				// since the axis list is sorted from lowest to highest then
				// we can say that everything after this is not overlaping
				break;
			}
		}
	}
	if(chosen.entity.id && pointOnEntity(stage, position, chosen.entity)) {
		return chosen.entity;
	};
	return {0, NONE};
}
