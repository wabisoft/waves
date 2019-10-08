#pragma once

#include <cassert>
#include <cmath>
#include <vector>

#include "vector2.hpp"

/********
 * Misc *
 ********/

template <typename T>
T sign(T t) {
	return (t == (T)0) ? (T)0 : t / abs(t);
}

template <typename T>
T signOf(T t) {
	return sign<T>(t);
}

/************
 * Geometry *
 ************/

bool lineSegmentIntersection(Vector2 a, Vector2 b, Vector2 c, Vector2 d, Vector2& intersection);
Vector2 findNormal(Vector2 a, Vector2 b, Vector2 c);

// returns + if point on left - if point on right 0 if point on line from b to a
inline float sideSign(Vector2 a, Vector2 b, Vector2 point) {
	Vector2 u = b - a;
	Vector2 v = point - a;
	float product = cross(u, v);
	return sign(product);
}

inline bool bounded(Vector2 a, Vector2 b, Vector2 point) {
	Vector2 ba = b - a;
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
