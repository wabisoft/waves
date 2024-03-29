#include "printing.hpp"
#include "aabb.hpp"
#include "maths.hpp"
#include "str.hpp"

using namespace glm;

std::ostream& operator<<(std::ostream& os, vec2 v) {
	return os << str(v);
	// return os << "[" << v[0] << ", " << v[1] << "]";
}

std::ostream& operator<<(std::ostream& os, AABB aabb) {
	return os << (int)aabb.id;
}

std::ostream& operator<<(std::ostream& os, sf::Vector2f v) {
	return os << "[" << v.x << ", " << v.y << "]";
}

