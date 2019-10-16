#pragma once

#include <cstdint>
#include "prelude.hpp"

enum EntityType : uint8_t {
	NONE		= 1 << 0, // 0000001
	SEA 		= 1 << 1, // 0000010
	PLATFORM 	= 1 << 2, // 0000100
	ROCK 		= 1 << 3, // 0001000
	SHIP		= 1 << 4, // 0010000
};// Good thing there aren't many entity types in the game :)

inline const char* str(EntityType type) {
	const char * names[] = {
		"", 							// 00000000
		"NONE", 						// 00000001
		"SEA", 							// 00000010
		"SEA|NONE", 					// 00000011
		"PLATFORM", 					// 00000100
		"PLATFORM|NONE",				// 00000101
		"PLATFORM|SEA",     			// 00000110
		"PLATFORM|SEA|NONE",			// 00000111
		"ROCK",							// 00001000
		"ROCK|NONE",					// 00001001
		"ROCK|SEA",						// 00001010
		"ROCK|SEA|NONE",				// 00001011
		"ROCK|PLATFORM",				// 00001100
		"ROCK|PLATFORM|NONE",			// 00001101
		"ROCK|PLATFORM|SEA",			// 00001110
		"ROCK|PLATFORM|SEA|NONE",		// 00001111
		"SHIP",							// 00010000
		"SHIP|NONE", 					// 00010001
		"SHIP|SEA", 					// 00010010
		"SHIP|SEA|NONE", 				// 00010011
		"SHIP|PLATFORM", 				// 00010100
		"SHIP|PLATFORM|NONE",			// 00010101
		"SHIP|PLATFORM|SEA",     		// 00010110
		"SHIP|PLATFORM|SEA|NONE",		// 00010111
		"SHIP|ROCK",					// 00011000
		"SHIP|ROCK|NONE",				// 00011001
		"SHIP|ROCK|SEA",				// 00011010
		"SHIP|ROCK|SEA|NONE",			// 00011011
		"SHIP|ROCK|PLATFORM",			// 00011100
		"SHIP|ROCK|PLATFORM|NONE",		// 00011101
		"SHIP|ROCK|PLATFORM|SEA",		// 00011110
		"SHIP|ROCK|PLATFORM|SEA|NONE",	// 00011111
	};
	return names[type];
}

struct Entity {
	// Specifically not a base class, more of a proxy or indirect reference
	// to things with ids and a type
	uint8_t id = 0;  // non-zero if refers to valid entity
	EntityType type = NONE;
};

Entity findEntityAtPosition(Stage& stage, Vector2 position);
Vector2 getEntityPosition(Entity entity);
// NOTE(owen): should we have a base entity class? revisit or don't. I can't decide.
