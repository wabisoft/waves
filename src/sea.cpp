#include "maths.hpp"
#include "sea.hpp"
#include "stage.hpp"
#include "wave.hpp"

uint8_t Sea::id_src = 0;

void updateSeas(Stage& stage) {
	for(Sea& sea : stage.seas) {
		updateWaves(stage, sea);
	}
}

uint8 createSea(Stage& stage, Vector2 position, float width, float height) {
	Sea new_sea;
	new_sea.shape = makeRectangle(position, width, height);
	new_sea.id = ++stage.id_src;
	stage.seas.push_back(new_sea);
	createAABB(stage, AABB(new_sea));
	return new_sea.id;
}

uint8 createSea(Stage& stage, Rectangle rect) {
	Sea new_sea;
	new_sea.shape = rect;
	new_sea.id = ++stage.id_src;
	stage.seas.push_back(new_sea);
	createAABB(stage, AABB(new_sea));
	return new_sea.id;
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

