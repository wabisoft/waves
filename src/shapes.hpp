#pragma once

#include "maths.hpp"

namespace wabi {

/**********
 * Shapes *
 **********/

struct Circle {
	glm::vec2 position;
	float radius = 0.f;
};

inline bool pointInsideCircle(glm::vec2 point, Circle& circle) {
	glm::vec2 relpos = point - circle.position;
	return glm::length(relpos) <= circle.radius;
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
	glm::vec2 position;
	glm::vec2 vertices[N];
	glm::vec2 model[N];
	int size = N;
	float rotation = 0.f;
};

struct Rectangle : Polygon<4>{
	Rectangle() {}
	Rectangle(glm::vec2 p, float w, float h, float r=0.f);

	float width = 0.f;
	float height = 0.f;
};

inline float area(const Circle & circle);
inline float area(Rectangle& rectangle);
template <int N>
inline void updateVertices(Polygon<N>& polygon);
template <int N>
inline bool pointInsidePolygon(glm::vec2 point, Polygon<N>& polygon);
template <int N>
inline glm::vec2 lower(const Polygon<N>& polygon); // gets the bottom left point of a polygon in relation to x, y axis
template <int N>
inline glm::vec2 upper(const Polygon<N>& polygon); // gets the top right point of a polygon in raltion to x, y axis
template <int N>
inline void boundingPoints(const Polygon<N>& polygon, glm::vec2 & lower, glm::vec2 & upper);


Rectangle makeRectangle(glm::vec2 p, float w, float h, float rotation=0.f);

} // namespace wabi

#include "shapes.inl" // definitions for template functions and inlines
