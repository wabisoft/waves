#include "maths.hpp"
#include "sea.hpp"
#include "shapes.hpp"
#include "stage.hpp"
#include "wave.hpp"

using namespace wabi;
using namespace glm;

uint8_t Sea::id_src = 0;

float Sea::heightAtX(float x) const {
	float height = upper(shape).y;
	for (const Wave& wave : waves) {
		height += wave.heightAtX(x);
	}
	return height;
}

vec2 Sea::velocityAtX(float x) const {
	vec2 velocity = VEC2_ZERO;
	for (const Wave& wave : waves) {
		velocity += wave.velocityAtX(x);
	}
	return velocity;
}

float Sea::slopeAtX(float x) const {
	float slope = 0;
	for (const Wave& wave : waves) {
		slope += wave.slopeAtX(x);
	}
	return slope;
}

void updateSeas(Stage& stage) {
	for(Sea& sea : stage.seas) {
		updateWaves(stage, sea);
	}
}

uint8 createSea(Stage& stage, vec2 position, float width, float height) {
	Sea sea = Sea(makeRectangle(width, height), position, ++stage.id_src);
	stage.seas.push_back(sea);
	createAABB(stage, AABB(sea));
	return sea.id;
}

uint8 createSea(Stage& stage, glm::vec2 position, Polygon& polygon) {
	Sea sea = Sea(polygon, position, ++stage.id_src);
	stage.seas.push_back(sea);
	createAABB(stage, AABB(sea));
	return sea.id;
}

SeaIt deleteSea(Stage& stage, SeaIt seaIt) {
	deleteAABB(stage, seaIt->id);
	return stage.seas.erase(seaIt);
}

SeaIt deleteSea(Stage& stage, uint8 seaId) {
	return deleteSea(stage, findSea(stage, seaId));
}

SeaIt findSea(Stage& stage, uint8 seaId) {
	SeaIt seaIt = std::lower_bound(stage.seas.begin(), stage.seas.end(), seaId, [](const Sea& s, uint8 id) -> bool { return s.id < id; });
	assert(seaIt != stage.seas.end());
	return seaIt;
}

