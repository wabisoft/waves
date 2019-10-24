#pragma once

#include <vector>

#include "constants.hpp"
#include "maths.hpp"

namespace wabi {

/**********
 * Shapes *
 **********/
// NOTE: !!!! Everything about our polygons assumes 2 things
// 1) a Polygon is convex (Concave polygons won't work for collision detection, and probably to a lesser degree our method of inclusion testing (pointInside checks)
// 2) a Polygon's model is arranged from top left first, clock wise to the last vertex
// i.e. Polygon vertices in the model/vertices vector in order from top left to topleft clockwise
// example:
//      index 0					index 1
//      	*---------------------*
//      	|                     |
//      	*---------------------*
//      index 3					 index 2


struct Polygon {
	void _reserve() {
		model.reserve(size);
		vertices.reserve(size);
		for(int i = 0; i < size; ++i) {
			model.push_back(glm::vec2(0));
			vertices.push_back(glm::vec2(0));
		}
	}

	Polygon() { _reserve(); }
	Polygon(int size): size(size), rotation(0) { _reserve(); }
	Polygon(int size, float rotation) : size(size), rotation(rotation) { _reserve(); }
	Polygon(std::vector<glm::vec2> model, float rotation) : vertices(model), model(model), rotation(rotation), size(model.size()) { }

	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> model;
	float rotation = 0.f;
	int size = 0;

};

void update(Polygon& polygon, glm::vec2 position, glm::mat3 transform = glm::mat3(1));
float minDistFromEdge(const glm::vec2 point, const Polygon& polygon, int& edgeStartIndex, bool& onVertex, float onVertexTolerance = 1.f);
bool pointInside(const glm::vec2 point, const Polygon& polygon);
float area(const Polygon& polygon);

const int CIRCLE_SIZE = 20;
const int RECTANGLE_SIZE = 4;
Polygon makeCircle(float radius, float rotation=0.f);
Polygon makeRectangle(float width, float height, float rotation=0.f);

inline glm::vec2 lower(const Polygon& polygon); // gets the bottom left point of a polygon in relation to x, y axis
inline glm::vec2 upper(const Polygon& polygon); // gets the top right point of a polygon in raltion to x, y axis
inline void boundingPoints(const Polygon& polygon, glm::vec2 & lower, glm::vec2 & upper);


// inline definitions

inline glm::vec2 lower(const Polygon& polygon) {
	float minX = INF;
	float minY = INF;
	for (int i = 0; i < polygon.size; ++i) {
		minX = std::fmin(polygon.vertices[i].x, minX);
		minY = std::fmin(polygon.vertices[i].y, minY);
	}
	return {minX, minY};
}

inline glm::vec2 lowerBound(const Polygon& polygon) {
	return lower(polygon);
}

inline glm::vec2 upper(const Polygon& polygon) {
	float maxX = -INF;
	float maxY = -INF;
	for (int i = 0; i < polygon.size; ++i) {
		maxX = std::fmax(polygon.vertices[i].x, maxX);
		maxY = std::fmax(polygon.vertices[i].y, maxY);
	}
	return {maxX, maxY};
}

inline glm::vec2 upperBound(const Polygon& polygon) {
	return upper(polygon);
}

inline void boundingPoints(const Polygon& polygon, glm::vec2 & lower, glm::vec2 & upper) {
	float minX = INF;
	float minY = INF;
	float maxX = -INF;
	float maxY = -INF;
	for (int i = 0; i < polygon.size; ++i) {
		minX = std::fmin(polygon.vertices[i].x, minX);
		minY = std::fmin(polygon.vertices[i].y, minY);
		maxX = std::fmax(polygon.vertices[i].x, maxX);
		maxY = std::fmax(polygon.vertices[i].y, maxY);
	}
	lower = {minX, minY};
	upper = {maxX, maxY};
}

} // namespace wabi
