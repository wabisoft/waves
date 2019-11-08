#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/ext/matrix_common.hpp>
#include <glm/ext/matrix_relational.hpp>

#include "settings.hpp"
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
	// boundingPoints(polygon, lower, upper);
}

float minDistFromEdge(const vec2 point, const Polygon& polygon, int& edgeStartIndex, bool& onVertex, float onVertexTolerance) {
	float min = INF;
	for (int i = 0; i < polygon.size; ++i) {
		vec2 a = polygon.vertices[i];
		vec2 b = polygon.vertices[((size_t)i+1) % polygon.vertices.size()];
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
	int stop = polygon.isChain ? polygon.size - 1 : polygon.size; // chains shouldn't close
	for (int i = 0; i < stop; ++i) {
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

// CHAIN
Polygon makeChain(int size, glm::vec2 start, glm::vec2 end, float rotation) {
	Polygon p = Polygon(size, rotation);
	p.model[0] = start;
	p.model[(size_t)size-1] = end;
	vec2 diff = end - start;
	vec2 step = diff / (float)size;
	for(int i = 1; i < size-1; i++) {
		p.model[i] = p.model[(size_t)i-1] + step;
	}
	p.isChain = true;
	return p;
}


std::vector<vec2> pointsOfIntersection(const wabi::Polygon p1, const wabi::Polygon& p2) {
	int p1stop = p1.isChain ? p1.size-1 : p1.size;
	int p2stop = p2.isChain ? p2.size-1 : p2.size;
	std::vector<vec2> intersectionPoints;
	for(int i = 0; i < p1stop; ++i) {
		auto a = p1.vertices[i];
		auto b = p1.vertices[(i+1)%p1.size];
		for(int j = 0; j < p2stop; ++j) {
			auto c = p2.vertices[j];
			auto d = p2.vertices[(j+1)%p2.size];
			vec2 intersect(0);
			float t = 0;
			if(lineSegmentIntersectionUsingAreas(a, b, c, d, t, intersect)){
				auto search = std::find(intersectionPoints.begin(), intersectionPoints.end(), intersect);
				if(search == intersectionPoints.end()) {
					intersectionPoints.push_back(intersect);
				}
			}
		}
	}
	return intersectionPoints;
}

std::vector<vec2> pointsOfIntersection(const vec2 lineStart, const vec2 lineEnd, const Polygon& polygon) {
	std::vector<vec2> intersectionPoints;
	for(int i = 0; i < polygon.size; ++i) {
		auto a = polygon.vertices[i];
		auto b = polygon.vertices[(i+1)%polygon.size];
		vec2 intersection(0);
		if(lineSegmentIntersection(a, b, lineStart, lineEnd, intersection)) {
			auto search = std::find(intersectionPoints.begin(), intersectionPoints.end(), intersection);
			if(search == intersectionPoints.end()) {
				intersectionPoints.push_back(intersection);
			}
		}
	}
	return intersectionPoints;
}

Polygon clip(const Polygon& a, const Polygon& b) {
	auto I = pointsOfIntersection(a, b);
	for(vec2 p : a.vertices) {
		if(pointInside(p, b)) {
			auto search = std::find(I.begin(), I.end(), p);
			if(search == I.end()) {
				I.push_back(p);
			}
		}
	}
	for(vec2 p : b.vertices) {
		if(pointInside(p, a)) {
			auto search = std::find(I.begin(), I.end(), p);
			if(search == I.end()) {
				I.push_back(p);
			}
		}
	}
	vec2 cent = centroid(I);
	std::sort(I.begin(), I.end(), [&cent](const vec2& v1, const vec2& v2) {
		bool res = isClockwise(cent, v1, v2);
		return res;
	});
	return Polygon(I, 0);
}

} // namespace wabi
