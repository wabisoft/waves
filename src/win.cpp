#include "stage.hpp"
#include "win.hpp"

uint8 createWin(Stage& stage, glm::vec2 position, float width, float height) {
	stage.win = Win(wabi::makeRectangle(width, height), position, ++stage.id_src);
	createAABB(stage, AABB(stage.win));
	return stage.win.id;
}

uint8 createWin(Stage& stage, glm::vec2 position, wabi::Polygon& shape) {
	stage.win = Win(shape, position, ++stage.id_src);
	createAABB(stage, AABB(stage.win));
	return stage.win.id;
}
