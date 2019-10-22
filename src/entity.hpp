#pragma once

#include <cstdint>

#include <glm/vec2.hpp>

#include "prelude.hpp"
#include "typedefs.hpp"
#include "shapes.hpp"

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
	uint8 id = 0;  // non-zero if refers to valid entity
	EntityType type = NONE;
};

struct EntityHandle {
	glm::vec2* pPosition;
	wabi::Polygon* pShape;
	EntityType type = NONE;
	uint8 id = 0;  // non-zero if refers to valid entity
};


bool pointOnEntity(Stage stage, glm::vec2 point, Entity entity);
EntityHandle findEntityAtPosition(Stage& stage, glm::vec2 position);
EntityHandle getEntityHandle(Stage& stage, Entity entity);
