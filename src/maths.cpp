#include "maths.hpp"

using namespace glm;

glm::vec2 operator*(const glm::mat3& m, const glm::vec2& v) {
	assert(m[2][2] == 1);
	glm::vec3 temp(v, 1);
	glm::vec3 prod = m * temp;
	assert(prod.z == 1);
	return { prod.x, prod.y };
}

inline float cross(glm::vec2 a, glm::vec2 b) {
	return  a.x*b.y - a.y*b.x;
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
}

float sideProduct(glm::vec2 a, glm::vec2 b, glm::vec2 point) {
	// returns mag of a perpendicular vector
	vec2 u = b - a;
	vec2 v = point - a;
	return cross(u, v);
}

float sideSign(glm::vec2 a, glm::vec2 b, glm::vec2 point) {
	// returns + if point on left - if point on right 0 if point on line from b to a
	return glm::sign(sideProduct(a, b, point));
}

bool bounded(glm::vec2 a, glm::vec2 b, glm::vec2 point) {
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

bool lineSegmentIntersection(vec2 a, vec2 b, vec2 c, vec2 d, vec2 & intersection) {
	// Return the point of intersection of 2 lineSegments
	// if return 0.f then no intersection
	// otherwise return depth of intersection and update intersection param
	// See line segment intersection in owen's notebook or online
	// http://www.cs.swan.ac.uk/~cssimon/line_intersection.html
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

vec2 findNormal(vec2 a, vec2 b, vec2 c) {
	// Returns the normal of the vector from a to b (b-a) in the direction of c
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

