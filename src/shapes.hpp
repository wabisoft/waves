#pragma once
#include "maths.hpp"

namespace wabi {

/**********
 * Shapes *
 **********/

struct Circle {
	Vector2 position;
	float radius = 0.f;
};

inline bool pointInsideCircle(Vector2 point, Circle& circle) {
	Vector2 relpos = point - circle.position;
	return magnitude(relpos) <= circle.radius;
}

// NOTE: Polygon vertices in the array in order from top left to topleft clockwise
// example:
//      index 0					index 1
//      	*---------------------*
//      	|                     |
//      	*---------------------*
//      index 3					 index 2
template <int N>
struct Polygon {
	Vector2 position;
	Vector2 vertices[N];
	Vector2 model[N];
	int size = N;
	float rotation = 0.f;
};

struct Rectangle : Polygon<4>{
	Rectangle() {}
	Rectangle(Vector2 p, float w, float h, float r=0.f);

	float width = 0.f;
	float height = 0.f;
};

inline float area(const Circle & circle);
inline float area(Rectangle& rectangle);
template <int N>
inline void updateVertices(Polygon<N>& polygon);
template <int N>
inline bool pointInsidePolygon(Vector2 point, Polygon<N>& polygon);
template <int N>
inline Vector2 lower(const Polygon<N>& polygon); // gets the bottom left point of a polygon in relation to x, y axis
template <int N>
inline Vector2 upper(const Polygon<N>& polygon); // gets the top right point of a polygon in raltion to x, y axis
template <int N>
inline void boundingPoints(const Polygon<N>& polygon, Vector2& lower, Vector2& upper);


Rectangle makeRectangle(Vector2 p, float w, float h, float rotation=0.f);

} // namespace wabi

#include "shapes.inl" // definitions for template functions and inlines
