#pragma once

#include "aabb.hpp"
#include "util.hpp"

struct Collider{	
	AABB aabb;
	void * entity;
	EntityType type;
};


