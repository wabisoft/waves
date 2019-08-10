#pragma once
#include <cstdint>

#include "entity.hpp"
#include "vector2.hpp"


struct AABB{
	inline AABB() {};
	inline AABB(const Vector2& lower, const Vector2& upper, EntityType type, int id) : lower(lower), upper(upper), type(type), id(id) { }

	Vector2 lower = {0.f,0.f};
	Vector2 upper = {0.f,0.f};
	EntityType type = NONE;

	// This works and is the simplest method for me to thing about. I'm sure
	// there is a more memory efficient way (that pointer is 8 bytes, on 64bit archs)
	// but I'm not gonna fret about that just yet
	void * entity = nullptr; 
	void (*updateCallback)(AABB&) = nullptr;
	uint8_t id = -1;
};


// TODO: program

