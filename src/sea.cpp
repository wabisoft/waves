#include "maths.hpp"
#include "sea.hpp"
#include "shapes.hpp"
#include "stage.hpp"
#include "wave.hpp"

using namespace wabi;
using namespace glm;

u8 Sea::id_src = 0;

float Sea::heightAtX(float x) const {
	// float height = upper(shape).y;
	float height = 0;
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
		for(int i = 0; i < sea.shape.size; ++i) {
			sea.shape.model[i].y = sea.heightAtX(sea.shape.vertices[i].x);
		}
		update(sea.shape, sea.position);
	}
}

// we probably want the sea chain size to vary by the actual length of the sea
// but...
const int SEA_CHAIN_SIZE = 20; // let's use this constant for now

u8 createSea(Stage& stage, vec2 position, float width, float height) {
	vec2 start = vec2(-(width/2), (height/2));
	vec2 end = vec2((width/2), -(height/2));
	Polygon shape = makeChain(SEA_CHAIN_SIZE, start, end);
	update(shape, position);
	Sea sea = Sea(shape, position, ++stage.id_src);
	stage.seas.push_back(sea);
	createAABB(stage, AABB(sea));
	return sea.id;
}

u8 createSea(Stage& stage, glm::vec2 position, Polygon& polygon) {
	// float width = polygon.model[1].x - polygon.model[0].x;
	// return createSea(stage, position, width, 0.f);
	Sea sea = Sea(polygon, position, ++stage.id_src);
	stage.seas.push_back(sea);
	createAABB(stage, AABB(sea));
	return sea.id;
}

SeaIt deleteSea(Stage& stage, SeaIt seaIt) {
	deleteAABB(stage, seaIt->id);
	return stage.seas.erase(seaIt);
}

SeaIt deleteSea(Stage& stage, u8 seaId) {
	return deleteSea(stage, findSea(stage, seaId));
}

SeaIt findSea(Stage& stage, u8 seaId) {
	SeaIt seaIt = std::lower_bound(stage.seas.begin(), stage.seas.end(), seaId, [](const Sea& s, u8 id) -> bool { return s.id < id; });
	assert(seaIt != stage.seas.end());
	return seaIt;
}

