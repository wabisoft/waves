#include "maths.hpp"

using namespace glm;

/******************
 * Geometry Stuff *
 ******************/

// Return the point of intersection of 2 lineSegments
// if return 0.f then no intersection
// otherwise return depth of intersection and update intersection param
// See line segment intersection in owen's notebook or online at http://www.cs.swan.ac.uk/~cssimon/line_intersection.html
bool lineSegmentIntersection(vec2 a, vec2 b, vec2 c, vec2 d, vec2 & intersection) {
	float h = (d.x - c.x) * (a.y - b.y) - (a.x - b.x) * (d.y - c.y);
	if (h == 0){
		return false; // h is zero when the lines are colinear
	}
	float t = ((a.x - c.x) * (a.y - b.y) + (a.y - c.y) * (b.x - a.x)) / h;
	float s = ((a.x - c.x) * (c.y - d.y) + (a.y - c.y) * (d.x - c.x)) / h;
	if (s >= 0.f && s < 1.f && t >=0.f && t < 1.f) {
		// assert(a + s * (b-a) == c + t * (d-c)); // assertion not safe because rounding error
		intersection = a + s * (b-a);
		return true;
	} else {
		return false;
	}
}


// Returns the normal of the vector from a to b (b-a) in the direction of c
vec2 findNormal(vec2 a, vec2 b, vec2 c) {
	vec2 u = b - a;
	vec2 v = c - a;
	float product = cross(u, v);
	if (product == 0) {
		// c is colinear/parallel to ba
		return {0.f, 0.f};
	} else if (product > 0) {
		// c is on the left of ba
		return {-u.y, u.x};  // {-dy, dx} is the left normal
	} else {
		// c is on the right
		return {u.y, -u.x};  // {dy, -dx} is the right normal
	}
}

vec2 & clamp(vec2& v, float s) {
	float mag = length(v);
	if (mag > s) {
		v /= mag;
		v *= s;
	}
	return v;
}

