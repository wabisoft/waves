#pragma once

#include "settings.hpp"
#include "shapes.hpp"

struct Win : Entity {
	Win () {}
	Win(wabi::Polygon shape, glm::vec2 position, u8 id) : Entity(Entity::WIN, shape, position, id) { }
	float timeInArea = 0;
	float timeToWin = 0.25;
};

u8 createWin(Stage& stage, glm::vec2 position, float width, float height);
u8 createWin(Stage& stage, glm::vec2 position, wabi::Polygon& shape);
