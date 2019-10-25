#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/ext/matrix_common.hpp>
#include <glm/ext/matrix_relational.hpp>

#include "constants.hpp"
#include "shapes.hpp"
#include "util.hpp"

using namespace glm;

namespace wabi {

// POLYGON
void update(Polygon& polygon, vec2 position, mat3 transform) {
	transform = translate(transform, position);
	transform = rotate(transform, polygon.rotation);
	for(int i = 0; i < polygon.size; ++i) {
		polygon.vertices[i] = transform * polygon.model[i];
	}
}

float minDistFromEdge(const vec2 point, const Polygon& polygon, int& edgeStartIndex, bool& onVertex, float onVertexTolerance) {
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

bool pointInside(const vec2 point, const Polygon& polygon) {
	for (int i = 0; i < polygon.size; ++i) {
		vec2 a = polygon.vertices[i];
		vec2 b = polygon.vertices[(i+1) % polygon.size];
		float sign = sideSign(a, b, point);
		// If the point is on the right (remeber we are going clock wise) then return false
		if (sign > 0) { // the > 0 here means that points on an edge are considered inside the polygon
			return false;
		}
	}
	return true;
}

float area(const Polygon& polygon) {
	float a = 0;
	for (int i = 0; i < polygon.size; ++i) {
		vec2 cur = polygon.model[i];
		vec2 next= polygon.model[(i+1)%polygon.size];
		a += (cur.y + next.y)/2 * (cur.x - next.x);
	}
	return std::fabs(a);
}
// CIRCLE

Polygon makeCircle(float radius, float rotation) {
	Polygon p(CIRCLE_SIZE, rotation);
	constexpr float step = (2.f * PI) / CIRCLE_SIZE;
	for(int i = 0 ; i < p.size; ++i) {
		float theta = step * i;
		// NOTE: negative cos makes the circle parameterization clockwise, which is important for our collision checking routine
		p.model[i] = vec2(-std::cos(theta), std::sin(theta)) * radius;
	}
	return p;
}

// RECTANGLE


Polygon makeRectangle(float width, float height, float rotation) {
	Polygon p = Polygon(RECTANGLE_SIZE, rotation);
	float halfWidth = width/2.f;
	float halfHeight = height/2.f;
	p.model[0] = {-halfWidth, halfHeight}; // top left
	p.model[1] = {halfWidth, halfHeight}; // top right
	p.model[2] = {halfWidth, -halfHeight}; // bottom right
	p.model[3] = {-halfWidth, -halfHeight}; // bottom left
	return p;
}

std::vector<vec2> pointsOfIntersection(const wabi::Polygon p1, const wabi::Polygon& p2) {
	std::vector<vec2> intersectionPoints;
	for(int i = 0; i < p1.size; ++i) {
		auto a = p1.vertices[i];
		auto b = p1.vertices[(i+1)%p1.size];
		for(int j = 0; j < p2.size; ++j) {
			auto c = p2.vertices[j];
			auto d = p2.vertices[(j+1)%p2.size];
			vec2 i(0);
			if(lineSegmentIntersection(a, b, c, d, i)){
				intersectionPoints.push_back(i);
			}
		}
	}
	return intersectionPoints;
}

} // namespace wabi
