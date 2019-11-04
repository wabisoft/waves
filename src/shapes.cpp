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
	// boundingPoints(polygon, lower, upper);
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
	p.model[size-1] = end;
	vec2 diff = end - start;
	vec2 step = diff / (float)size;
	for(int i = 1; i < size-1; i++) {
		p.model[i] = p.model[i-1] + step;
	}
	p.isChain = true;
	return p;
}


std::vector<vec2> pointsOfIntersection(const wabi::Polygon p1, const wabi::Polygon& p2) {
	float p1stop = p1.isChain ? p1.size-1 : p1.size;
	float p2stop = p2.isChain ? p2.size-1 : p2.size;
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

std::vector<vec2> andrewHull(std::vector<vec2> P) {
	size_t n = P.size(), k = 0;
	if (n <= 3) return P;
	std::vector<vec2> H(2*n);

	// Sort points lexicographically
	std::sort(P.begin(), P.end(), [](const vec2& a, const vec2& b) {
		// NOTE: this ordering says:
		// sort from left to right and (if tie) top to bottom
		// that means our first pass will get the upper hull
		// and our second pass will be the lower hull
		return a.x < b.x || (a.x == b.x && a.y > b.y);
	});

	// Build upper hull
	for (size_t i = 0; i < n; ++i) {
		while (k >= 2 && sideProduct(H[k-1], P[i], H[k-2]) <= 0) k--;
		H[k++] = P[i];
	}

	// Build upper hull
	for (size_t i = n-1, t = k+1; i > 0; --i) {
		while (k >= t && sideProduct(H[k-1], P[i-1], H[k-2]) <= 0) k--;
		H[k++] = P[i-1];
	}

	H.resize(k-1);
	// H is counter-clockwise, and our polygons should wind clockwise, so we reverse it
	// std::reverse(H.begin(), H.end());
	return H;
}

Polygon clipChainWithPolygon(const Polygon& a, const Polygon& chain) {
	std::vector<vec2> I;
	std::vector<vec2> chainSubSet;
	std::vector<vec2>::const_iterator start = chain.vertices.end();
	std::vector<vec2>::const_iterator end = chain.vertices.end();
	for(auto it = chain.vertices.begin(); it != chain.vertices.end(); it++) {
		if(pointInside(*it, a)) {
			start = it;
			end = ++it;
			while(it != chain.vertices.end()) {
				if(pointInside(*it, a)) {
					end = ++it;
				} else {
					break;
				}
			}
			break;
		}
	}
	if(start == chain.vertices.end()) { return Polygon();}
	I.insert(I.end(), start, end);
	start = start == chain.vertices.begin() ? start : std::prev(start);
	end = end == chain.vertices.end() ? end : std::next(end);
	chainSubSet.insert(chainSubSet.end(), start, end);
	for (auto it = a.vertices.begin(); it != a.vertices.end(); ++it) {
		bool include = true;
		for (auto cit = chainSubSet.begin(); cit != std::prev(chainSubSet.end()); ++cit) {
			auto c = *cit;
			auto n = *std::next(cit);
			include &= sideSign(c, n, *it) <= 0;
			auto nit = std::next(it) == a.vertices.end() ? a.vertices.begin() : std::next(it);
			vec2 intersect(0);
			if (lineSegmentIntersection(*it, *nit, c, n, intersect)) {
				float zero = 0.1f;
				if(squaredLength(*it - intersect) > zero && squaredLength(*nit - intersect) > zero) {
					I.push_back(intersect);
				}
			}
		}
		if (include) {
			I.push_back(*it);
		}
	}
	I = andrewHull(I);
	return Polygon(I, 0);
}

Polygon clipPolygonWithPolygon(const Polygon& a, const Polygon& b) {
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

Polygon clip(const Polygon& a, const Polygon& b) {
	if(a.isChain) { return clipChainWithPolygon(b, a); }
	else if(b.isChain) { return clipChainWithPolygon(a, b); }
	else { return clipPolygonWithPolygon(a, b); }
}

} // namespace wabi
