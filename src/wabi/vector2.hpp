#pragma once

#include <iostream>
//  This is the vector2 class for namespace wabi. It's a fairly simple vector2 implementation

namespace wabi{
class Vector2{
public:
	Vector2();
	Vector2(float, float);
	Vector2(const Vector2&);


	float magnitude() const;
	float squaredMagnitude() const;
	float dot(const Vector2& other) const;
	Vector2 normalized() const; // normalize copy
	Vector2& normalize(); // normalize in place
	float& operator[](int i); // read
	const float& operator[](int i) const; // write

	float x;
	float y;
};

// Vector negation
Vector2 operator-(const Vector2&);

// Vector/Vector addition and subtraction
Vector2 operator+(const Vector2&, const Vector2&);
Vector2 operator-(const Vector2&, const Vector2&);
Vector2& operator+=(Vector2&, const Vector2&);
Vector2& operator-=(Vector2&, const Vector2&);

// Vector equality an inequality
bool operator==(const Vector2&, const Vector2&);
bool operator!=(const Vector2&, const Vector2&);

// Vector/Scalar multiplication and addition
template <typename T>
Vector2 operator*(const Vector2&, T);
template <typename T>
Vector2 operator*(T, const Vector2&);
template <typename T>
Vector2 operator/(const Vector2&, T);
template <typename T>
Vector2 operator/(T, const Vector2&);
template <typename T>
Vector2& operator*=(Vector2&, T);
template <typename T>
Vector2& operator/=(Vector2&, T);


#include "vector2.inl"

}; // namespace wabi

std::ostream& operator<<(std::ostream&, wabi::Vector2&);
