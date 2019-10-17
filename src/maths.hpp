#pragma once

#include <cassert>
#include <cmath>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/mat3x3.hpp>

const glm::vec2 VEC2_UP {0.f, 1.f};
const glm::vec2 VEC2_DOWN {0.f, -1.f};
const glm::vec2 VEC2_LEFT {-1.f, 0.f};
const glm::vec2 VEC2_RIGHT {1.f, 0.f};
const glm::vec2 VEC2_ZERO {0.f, 0.f};

const glm::mat3 MAT3_IDENTITY(1);


/********
 * Misc *
 ********/

// template <typename T>
// T sign(T t) {
// 	return (t == (T)0) ? (T)0 : t / abs(t);
// }

// template <typename T>
// T signOf(T t) {
// 	return glm::sign<T>(t);
// }

/************
 * Geometry *
 ************/

bool lineSegmentIntersection(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d, glm::vec2 & intersection);
glm::vec2 findNormal(glm::vec2 a, glm::vec2 b, glm::vec2 c);
glm::vec2& clamp(glm::vec2& v, float s); // clamp this vector to a scalar magnitude

/************************************************************
 * SPECIAL CASE OF 2d cross product							*
 * NOTE: remember that |a x b| = |a| |b| sin(theta)			*
 * that means for a 2d cross we have:                    	*
 * a x b = {												*
 * 		a.y*b.z - a.z*b.y,									*
 * 		a.z*b.x - a.x*b.z,									*
 * 		a.x*b.y - a.y*b.x									*
 * }														*
 *															*
 * in 2d the z is 0 so we get a new 3d vector				*
 * the magnitude of that vector is just the z component		*
 * That value is also |a||b|sin(theta)						*
 * which means that the sign of theta (+/-) can tell us 	*
 * if we have a counter-clockwise(-) or clockwise(+)		*
 * rotation between the 2 vectors.							*
 * this important to determine,	which side of a line a		*
 * point is on among other things							*
************************************************************/
inline float cross(glm::vec2 a, glm::vec2 b) {
	return  a.x*b.y - a.y*b.x;
}

// returns + if point on left - if point on right 0 if point on line from b to a
inline float sideSign(glm::vec2 a, glm::vec2 b, glm::vec2 point) {
	glm::vec2 u = b - a;
	glm::vec2 v = point - a;
	float product = cross(u, v);
	return glm::sign(product);
}

inline bool bounded(glm::vec2 a, glm::vec2 b, glm::vec2 point) {
	glm::vec2 ba = b - a;
	if (std::abs(ba.x) > std::abs(ba.y)) { // line more horizontally oriented
		// we check that we are bounded on x
		if (ba.x > 0) { // positive dx means b.x > a.x
			return point.x >= a.x && point.x <=b.x;
		} else { // negative dx means that b.x < a.x
			return point.x >= b.x && point.x <=a.x;
		}
	} else { // line is more vertically oriented
		// we check that we are bounded on y
		if (ba.y > 0) { // positive dy means b.y > a.y
			return point.y >= a.y && point.y <= b.y;
		} else { // negative dy means that a.y > b.y
			return point.y >= b.y && point.y <= a.y;
		}
	}
	assert(false);
}

