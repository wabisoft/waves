#pragma once

#include "vector2.hpp"
#include "util.hpp"


struct AABB{
	inline AABB(const Vector2& lower, const Vector2& upper, EntityType type, int id) : lower(lower), upper(upper), type(type), id(id) { }

	Vector2 lower = {0,0};
	Vector2 upper = {0,0};
	EntityType type;
	int id;
};



// TODO: program
