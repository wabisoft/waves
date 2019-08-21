#pragma once
#include <cstdint>

#include "entity.hpp"
#include "prelude.hpp"
#include "maths.hpp"


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
int deleteAABBByIdx(Stage& stage, int aabb_idx);
int deleteAABBById(Stage& stage, uint8_t aabb_id);

inline bool operator==(const AABB& a, const AABB& b) {
		return a.id == b.id && a.type == b.type; // note that we do not compare vectors because floats are all shitty
}

struct AABBPair{
	AABB a;
	AABB b;
};

inline bool operator==(const AABBPair& a, const AABBPair& b){
	return a.a == b.a && a.b == b.b;
}

// TODO: program

