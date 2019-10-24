#pragma once
#include <cstdint>
#include <glm/vec2.hpp>

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
	explicit AABB (const Win&);
	glm::vec2 lower = {0.f,0.f};
	glm::vec2 upper = {0.f,0.f};
	Entity::Type type = Entity::NONE;
	uint8 id = 0;
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

enum Axis : uint8 {
	NO_AXIS = 0,
	X_AXIS = 1 << 0,
	Y_AXIS = 1 << 1,
	BOTH_AXES = 3
};

struct AABBPair{
	Axis overlap = NO_AXIS;
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
	return pair.overlap == (X_AXIS | Y_AXIS);
}

inline bool operator==(const AABBPair& a, const AABBPair& b){
	return a.a == b.a && a.b == b.b;
}

// TODO: program

