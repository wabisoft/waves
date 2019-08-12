#pragma once
#include <cstdint>

#include "entity.hpp"
#include "prelude.hpp"
#include "vector2.hpp"


struct AABB{
	inline AABB() {};
	explicit AABB (const Rock&);
	explicit AABB (const Platform&);
	explicit AABB (const Ship&);
	explicit AABB (const Sea&);
	Vector2 lower = {0.f,0.f};
	Vector2 upper = {0.f,0.f};
	EntityType type = NONE;
	uint8_t id = 0;
};

uint8_t createAABB(Stage& stage, AABB aabb);
size_t deleteAABBByIdx(Stage& stage, int aabb_idx);
size_t deleteAABBById(Stage& stage, uint8_t aabb_id);

// TODO: program

