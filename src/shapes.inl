// maths.hpp

#include "constants.hpp"

inline float area(const Circle& circle) {
	return circle.radius * circle.radius * PI;
}

inline float area(const Rectangle& rectangle) {
	return rectangle.width * rectangle.height;
}

template <int N>
bool pointInsidePolygon(Vector2 point, Polygon<N>& polygon) {
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
