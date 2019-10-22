#pragma once

#include <vector>

#include "maths.hpp"

namespace wabi {

/**********
 * Shapes *
 **********/

struct Polygon {
	// NOTE: Polygon vertices in the model/vertices vector in order from top left to topleft clockwise
	// example:
	//      index 0					index 1
	//      	*---------------------*
	//      	|                     |
	//      	*---------------------*
	//      index 3					 index 2
	void _reserve() {
		model.reserve(size);
		vertices.reserve(size);
		for(int i = 0; i < size; ++i) {
			model.push_back(glm::vec2(0));
			vertices.push_back(glm::vec2(0));
		}
	}

	Polygon() { _reserve(); }
	Polygon(int size): size(size), position(0), rotation(0) { _reserve(); }
	Polygon(int size, glm::vec2 position, float rotation) : size(size), position(position), rotation(rotation) { _reserve(); }

	glm::vec2 position;
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> model;

	float rotation = 0.f;
	int size = 0;

};

void update(Polygon& polygon, glm::mat3 transform = glm::mat3(1));

float minDistFromEdge(const glm::vec2 point, const Polygon& polygon, int& edgeStartIndex, bool& onVertex, float onVertexTolerance = 1.f);
bool pointInside(const glm::vec2 point, const Polygon& polygon);


inline glm::vec2 lower(const Polygon& polygon); // gets the bottom left point of a polygon in relation to x, y axis
inline glm::vec2 upper(const Polygon& polygon); // gets the top right point of a polygon in raltion to x, y axis
inline void boundingPoints(const Polygon& polygon, glm::vec2 & lower, glm::vec2 & upper);

// CIRCLE

const int CIRCLE_SIZE = 20;

struct Circle : Polygon{
	Circle() : Polygon(CIRCLE_SIZE) {
		model.reserve(size);
		vertices.reserve(size);
	}
	Circle(glm::vec2 pos, float rad, float rot=0.f);

	float radius = 0.f;
};

// void update(Circle& circle, glm::mat3 transform = glm::mat3(1));
bool pointInside(const glm::vec2 point, const Circle& circle);
float area(const Circle& circle);

// RECTANGLE

const int RECTANGLE_SIZE = 4;

struct Rectangle : Polygon {
	Rectangle() : Polygon(RECTANGLE_SIZE) {}
	Rectangle(glm::vec2 p, float w, float h, float r=0.f);

	float width = 0.f;
	float height = 0.f;
};

float area(const Rectangle& rectangle);
Rectangle makeRectangle(glm::vec2 p, float w, float h, float rotation=0.f);

} // namespace wabi

#include "shapes.inl" // definitions for template functions and inlines
