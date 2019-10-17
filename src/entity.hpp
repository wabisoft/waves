#pragma once

#include <cstdint>

#include <glm/vec2.hpp>

#include "prelude.hpp"

enum EntityType : uint8_t {
	NONE		= 1 << 0, // 0000001
	SEA 		= 1 << 1, // 0000010
	PLATFORM 	= 1 << 2, // 0000100
	ROCK 		= 1 << 3, // 0001000
	SHIP		= 1 << 4, // 0010000
};// Good thing there aren't many entity types in the game :)

struct Entity {
	// Specifically not a base class, more of a proxy or indirect reference
	// to things with ids and a type
	uint8_t id = 0;  // non-zero if refers to valid entity
	EntityType type = NONE;
};

Entity findEntityAtPosition(Stage& stage, glm::vec2 position);
Entity findEntityAtPosition(Stage& stage, glm::vec2 position, glm::vec2& entityPosition);
glm::vec2 getEntityPosition(Entity entity);
