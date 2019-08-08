#include "printing.hpp"
#include "aabb.hpp"
#include "vector2.hpp"

std::ostream& operator<<(std::ostream& os, const Vector2& v) {
	return os << "(" << v[0] << ", " << v[1] << ")";
}

std::ostream& operator<<(std::ostream& os, const AABB& aabb) {
	return os << "{ " << aabb.lower << ", " << aabb.upper << ", " << aabb.type << " }";
}
