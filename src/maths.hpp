#pragma once

#include <cassert>
#include <cmath>
#include <vector>

#include "vector2.hpp"

template <typename T>
T sign(T t) {
	return t / abs(t);
}

struct Circle {
	Vector2 position;
	float radius = 0.f;
};

template <int N>
struct Polygon {
	Vector2 position;
	Vector2 vertices[N];
	Vector2 model[N];
	const int size = N;
	float rotation = 0.f;
};

struct Rectangle : Polygon<4>{
	float width = 0.f;
	float height = 0.f;
};


inline Rectangle makeRectangle(Vector2 p, float w, float h) {
	Rectangle r;
	r.width = w;
	r.height = h;
	r.position = p;
	float halfWidth = w/2.f;
	float halfHeight = h/2.f;
	r.model[0] = {-halfWidth, halfHeight}; // top left
	r.model[1] = {halfWidth, halfHeight}; // top right
	r.model[2] = {halfWidth, -halfHeight}; // bottom right
	r.model[3] = {-halfWidth, -halfHeight}; // bottom left
	r.vertices[0] = r.position + r.model[0];
	r.vertices[1] = r.position + r.model[1];
	r.vertices[2] = r.position + r.model[2];
	r.vertices[3] = r.position + r.model[3];
	return r;
}


// Return the point of intersection of 2 lineSegments
// if return 0.f then no intersection
// otherwise return depth of intersection and update intersection param
// See line segment intersection in owen's notebook or online at http://www.cs.swan.ac.uk/~cssimon/line_intersection.html
inline bool lineSegmentIntersection(Vector2 a, Vector2 b, Vector2 c, Vector2 d, Vector2& intersection) {
	float h = (d.x - c.x) * (a.y - b.y) - (a.x - b.x) * (d.y - c.y);
	if (h == 0){
		return false; // h is zero when the lines are colinear
	}
	float t = ((a.x - c.x) * (a.y - b.y) + (a.y - c.y) * (b.x - a.x)) / h;
	float s = ((a.x - c.x) * (c.y - d.y) + (a.y - c.y) * (d.x - c.x)) / h;
	if (s >= 0.f && s < 1.f && t >=0.f && t < 1.f) {
		assert(a + s * (b-a) == c + t * (d-c));
		intersection = a + s * (b-a);
		return true;
	} else {
		return false;
	}
}


