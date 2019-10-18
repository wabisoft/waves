// shapes.hpp

#include "constants.hpp"
namespace wabi {

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
