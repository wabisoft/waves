#pragma once
#include <cstdint>
#include "prelude.hpp"

enum EntityType : uint8_t {
	NONE		= 1 << 1,
	SEA 		= 1 << 2,
	PLATFORM 	= 1 << 3,
	ROCK 		= 1 << 4,
	SHIP		= 1 << 5,
};// Good thing there aren't many entity types in the game :)


struct Entity {
	// Specifically not a base class, more of a proxy or indirect reference
	// to things with ids and a type
	uint8_t id = 0;  // non-zero if refers to valid entity
	EntityType type = NONE;
};

// NOTE(owen): should we have a base entity class? revisit or don't. I can't decide.
