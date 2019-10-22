#include <glm/gtx/matrix_transform_2d.hpp>
#include "shapes.hpp"
#include <glm/ext/matrix_common.hpp>
#include <glm/ext/matrix_relational.hpp>

using namespace glm;

namespace wabi {
// POLYGON
void update(Polygon& polygon, mat3 transform) {
	// Since I don't feel like making a matrix struct and writing a bunch of other
	// code to model matrix math, here is our rotation matrix
	transform = glm::translate(transform, polygon.position);
	transform = glm::rotate(transform, polygon.rotation);
	// glm::vec2 xRotation = {std::cos(rotation), std::sinf(rotation)};
	// glm::vec2 yRotation = {-std::sin(rotation), std::cosf(rotation)};
	for(int i = 0; i < polygon.size; ++i) {
		// rotation plus transformation
		// vertices[i] = position + model[i].x * xRotation + model[i].y * yRotation;
		polygon.vertices[i] = transform * polygon.model[i];
	}
}

float minDistFromEdge(const glm::vec2 point, const Polygon& polygon, int& edgeStartIndex, bool& onVertex, float onVertexTolerance) {
	float min = INF;
	for (int i = 0; i < polygon.size; ++i) {
		vec2 a = polygon.vertices[i];
		vec2 b = polygon.vertices[(i+1) % polygon.vertices.size()];
		vec2 ba = b - a;
		vec2 pa = point - a;
		float pa_d = length(pa);
		bool _onVert = false;
		if(pa_d <= onVertexTolerance) {
			_onVert = true;
		}
		float len_ba = length(ba);
		vec2 pp = (dot(pa, ba) / len_ba) * (ba/len_ba) ;
		pp = a + pp;
		float dist = std::fabs(length(point - pp));
		if(min > dist) {
			min = dist;
			edgeStartIndex = i;
			onVertex = _onVert;
		}
		min = min > dist ? dist : min;
	}
	return min;
}

bool pointInside(const glm::vec2 point, const Polygon& polygon) {
	for (int i = 0; i < polygon.size; ++i) {
		glm::vec2 a = polygon.vertices[i];
		glm::vec2 b = polygon.vertices[(i+1) % polygon.size];
		float sign = sideSign(a, b, point);
		// If the point is on the right (remeber we are going clock wise) then return false
		if (sign > 0) { // the > 0 here means that points on an edge are considered inside the polygon
			return false;
		}
	}
	return true;
}

// CIRCLE

Circle::Circle(vec2 pos, float rad, float rot) : Polygon(CIRCLE_SIZE, pos, rot), radius(rad) {
	constexpr float step = 1.f / 20;
	for(int i = 0 ; i < size; ++i) {
		float theta = 2.f * PI * step * i;
		// NOTE: negative cos makes the circle parameterization clockwise, which is important for our collision checking routine
		model[i] = vec2(-std::cos(theta), std::sin(theta)) * radius;
	}
	update(*this); // update vertices from model
}

// void update(Circle& circle, mat3 transform) {
	// transform = glm::scale(transform, vec2(circle.radius, circle.radius));
	// update(reinterpret_cast<Polygon&>(circle), transform);
// }

bool pointInside(const glm::vec2 point, const Circle& circle) {
	glm::vec2 relpos = point - circle.position;
	return glm::length(relpos) <= circle.radius;
}

float area(const Circle& circle) {
	return circle.radius * circle.radius * PI;
}

// RECTANGLE

float area(const Rectangle& rect) {
	return rect.width * rect.height;
}

Rectangle::Rectangle(vec2 p, float w, float h, float r) : Polygon(RECTANGLE_SIZE, p, r), width(w), height(h) {
	float halfWidth = w/2.f;
	float halfHeight = h/2.f;
	model[0] = {-halfWidth, halfHeight}; // top left
	model[1] = {halfWidth, halfHeight}; // top right
	model[2] = {halfWidth, -halfHeight}; // bottom right
	model[3] = {-halfWidth, -halfHeight}; // bottom left
	update(*this); // update vertices from model
}

Rectangle makeRectangle(vec2 p, float w, float h, float rotation) {
	return Rectangle(p, w, h, rotation);
}

} // namespace wabi
