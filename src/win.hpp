#pragma once

#include "shapes.hpp"
#include "typedefs.hpp"

struct Win : Entity {
	Win () {}
	Win(wabi::Polygon shape, glm::vec2 position, uint8 id) : Entity(Entity::WIN, shape, position, id) { }
	float timeInArea = 0;
	float timeToWin = 0.25;
};

uint8 createWin(Stage& stage, glm::vec2 position, float width, float height);
uint8 createWin(Stage& stage, glm::vec2 position, wabi::Polygon& shape);
