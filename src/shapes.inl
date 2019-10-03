// shapes.hpp

#include "constants.hpp"
namespace wabi {
inline float area(const Circle& circle) {
	return circle.radius * circle.radius * PI;
}

inline float area(const Rectangle& rectangle) {
	return rectangle.width * rectangle.height;
}

template <int N>
inline void updateVertices(Polygon<N>& polygon){
	// Since I don't feel like making a matrix struct and writing a bunch of other
	// code to model matrix math, here is our rotation matrix
	Vector2 xRotation = {std::cos(polygon.rotation), std::sinf(polygon.rotation)};
	Vector2 yRotation = {-std::sin(polygon.rotation), std::cosf(polygon.rotation)};
	for(int i = 0; i < polygon.size; ++i) {
		// rotation plus transformation
		polygon.vertices[i] = polygon.position + polygon.model[i].x * xRotation + polygon.model[i].y * yRotation;
	}
}


template <int N>
inline bool pointInsidePolygon(Vector2 point, Polygon<N>& polygon) {
	for (int i = 0; i < polygon.size; ++i) {
		Vector2 a = polygon.vertices[i];
		Vector2 b = polygon.vertices[(i+1) % polygon.size];
		float sign = sideSign(a, b, point);
		// If the point is on the right (remeber we are going clock wise) then return false
		if (sign > 0) { // the > 0 here means that points on an edge are considered inside the polygon
			return false;
		}
	}
	return true;
}

template <int N>
inline Vector2 lower(const Polygon<N>& polygon) {
	float minX = INF;
	float minY = INF;
	for (int i = 0; i < polygon.size; ++i) {
		minX = std::fmin(polygon.vertices[i].x, minX);
		minY = std::fmin(polygon.vertices[i].y, minY);
	}
	return {minX, minY};
}

template <int N>
inline Vector2 upper(const Polygon<N>& polygon) {
	float maxX = -INF;
	float maxY = -INF;
	for (int i = 0; i < polygon.size; ++i) {
		maxX = std::fmax(polygon.vertices[i].x, maxX);
		maxY = std::fmax(polygon.vertices[i].y, maxY);
	}
	return {maxX, maxY};
}

template <int N>
inline void boundingPoints(const Polygon<N>& polygon, Vector2& lower, Vector2& upper) {
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
