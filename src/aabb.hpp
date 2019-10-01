#pragma once
#include <cstdint>

#include "entity.hpp"
#include "prelude.hpp"
#include "maths.hpp"
#include "typedefs.hpp"


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

typedef std::vector<AABB>::iterator AABBIt;

void updateAABBS(Stage& stage);
uint8_t createAABB(Stage& stage, AABB aabb);
AABBIt findAABB(Stage& stage, uint8 aabbId);
AABBIt deleteAABB(Stage& stage, AABBIt aabbIt);
AABBIt deleteAABB(Stage& stage, uint8 aabbId);

inline float area(const AABB aabb) {
	return (aabb.upper.x - aabb.lower.x) * (aabb.upper.y - aabb.lower.y);
}

inline bool operator==(const AABB& a, const AABB& b) {
		return a.id == b.id && a.type == b.type; // note that we do not compare vectors because floats are all shitty
}

struct AABBPair{
	enum Axis : uint8 {
		NONE = 0,
		X_AXIS = 1 << 0,
		Y_AXIS = 1 << 1,
		BOTH = 3
	};

	Axis overlap = NONE;
	AABB a;
	AABB b;

	void setAxis(Axis axis) {
		overlap = (Axis)(overlap | axis);
	}
	void unsetAxis(Axis axis) {
		overlap = (Axis)(overlap & ~axis);
	}
};

inline bool collides(AABBPair pair) {
	return pair.overlap == (AABBPair::X_AXIS | AABBPair::Y_AXIS);
}

inline bool operator==(const AABBPair& a, const AABBPair& b){
	return a.a == b.a && a.b == b.b;
}

// TODO: program

