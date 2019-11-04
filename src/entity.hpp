#pragma once

#include <cstdint>

#include <glm/vec2.hpp>

#include "prelude.hpp"
#include "typedefs.hpp"
#include "shapes.hpp"

// Good thing there aren't many entity types in the game :)


struct Entity {
	enum Type : uint8_t {
		NONE		= 1 << 0, // 0000001
		SEA 		= 1 << 1, // 0000010
		PLATFORM 	= 1 << 2, // 0000100
		ROCK 		= 1 << 3, // 0001000
		SHIP		= 1 << 4, // 0010000
		WIN			= 1 << 5, // 0100000
	};

	Entity () : shape(wabi::Polygon()), position(glm::vec2(0)) {}
	Entity (Type type, wabi::Polygon& shape, glm::vec2 position, uint8 id)
		: shape(shape), position(position), id(id), type(type) { }
	wabi::Polygon shape;
	glm::vec2 position;
	uint8 id = 0;  // non-zero if refers to valid entity
	Type type = NONE;
};

// bool pointOnEntity(Stage stage, glm::vec2 point, Entity entity);
Entity* findEntityAtPosition(Stage& stage, glm::vec2 position);
Entity* getEntity(Stage& stage, uint8 id, Entity::Type type);
