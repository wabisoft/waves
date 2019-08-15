#include "printing.hpp"
#include "aabb.hpp"
#include "maths.hpp"

std::ostream& operator<<(std::ostream& os, const Vector2& v) {
	return os << "(" << v[0] << ", " << v[1] << ")";
}

std::ostream& operator<<(std::ostream& os, const AABB& aabb) {
	return os << (int)aabb.id;
}
