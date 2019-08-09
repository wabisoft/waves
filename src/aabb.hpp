#pragma once

#include "entity.hpp"
#include "vector2.hpp"


struct AABB{
	inline AABB() {};
	inline AABB(const Vector2& lower, const Vector2& upper, EntityType type, int id) : lower(lower), upper(upper), type(type), id(id) { }

	Vector2 lower = {0,0};
	Vector2 upper = {0,0};
	EntityType type = NONE;

	void * entity = nullptr; // using type we can cast to whatever entity maybe? TODO this feel awkward and I'm not sure
	int id = -1;
};


// TODO: program

