#include <cmath>
#include "vector2.hpp"

namespace wabi{

Vector2::Vector2(): x(0.f), y(0.f) {}
Vector2::Vector2(float x, float y): x(x), y(y) {}
Vector2::Vector2(const Vector2& v): x(v.x), y(v.y) {}

float Vector2::magnitude() const{
	return std::sqrt(squaredMagnitude());
}

float Vector2::squaredMagnitude() const {
	return this->dot(*this);
}

float Vector2::dot(const Vector2& v) const {	
	return x * v.x + y * v.y;
}

Vector2 Vector2::normalized() const {
	return *this / magnitude();
}

Vector2& Vector2::normalize() {
	return *this /= magnitude();
}

float& Vector2::operator[](int i){
	return ((&x)[i]);
}

const float& Vector2::operator[](int i) const {
	return ((&x)[i]);
}


Vector2 operator-(const Vector2& v){
	return Vector2(-v.x, -v.y);
}

Vector2 operator+(const Vector2& a, const Vector2& b) {
	return Vector2(a.x + b.x, a.y + b.y);
}

Vector2 operator-(const Vector2& a, const Vector2& b) {
	return Vector2(a.x - b.x, a.y - b.y);
}

Vector2& operator+=(Vector2& a, const Vector2& b) {
	a.x += b.x;
	a.y += b.y;
	return a;
}

Vector2& operator-=(Vector2& a, const Vector2& b) {
	a.x -= b.x;
	a.y -= b.y;
	return a;
}


bool operator==(const Vector2& a, const Vector2& b) {
	return a.x == b.x && a.y == b.y;	
}

bool operator!=(const Vector2& a, const Vector2& b) {
	return a.x != b.x || a.y != b.y;	
}

}; // namespace wabi

std::ostream& operator<<(std::ostream& os, wabi::Vector2& v) {
	return os << "(" << v.x << ", " << v.y << ")";
}

