#pragma once
#include <math.h>

//  This is the vector2 class for namespace wabi. It's a fairly simple vector2 implementation


struct Vector2{
	inline Vector2() {}
	inline Vector2(float x, float y): x(x), y(y) { }
	inline Vector2(const Vector2& v): x(v[0]), y(v[1]) { }

	inline float& operator[](int i) { return (&x)[i]; } // read
	inline const float& operator[](int i) const { return (&x)[i]; }; // write

	float x, y = 0;
};

// Vector negation
inline Vector2 operator-(const Vector2& v) {
	return Vector2(-v[0], -v[1]);
}

// Vector/Vector addition and subtraction
inline Vector2 operator+(const Vector2& b, const Vector2& a) {
	return Vector2(a[0] + b[0], a[1] + b[1]);

}

inline Vector2 operator-(const Vector2& a, const Vector2& b) {
	return Vector2(a[0] - b[0], a[1] - b[1]);
}

inline Vector2& operator+=(Vector2& a, const Vector2& b) {
	a[0] += b[0];
	a[1] += b[1];
	return a;
}

inline Vector2& operator-=(Vector2& a, const Vector2& b) {
	a[0] -= b[0];
	a[0] -= b[1];
	return a;
}

// Vector equality an inequality
inline bool operator==(const Vector2& a, const Vector2& b) {
	return a[0] == b[0] && a[1] == b[1];
}

inline bool operator!=(const Vector2& a, const Vector2& b) {
	return a[0] != b[0] || a[1] != b[1];
}


// Vector/Scalar multiplication and addition
inline Vector2 operator*(const Vector2& v, float s) {
	return Vector2(v[0] * s, v[1] * s);
}

inline Vector2 operator*(float s, const Vector2& v) {
	return Vector2(v[0] * s, v[1] * s);
}

inline Vector2 operator/(const Vector2& v, float s) {
	return Vector2(v[0] / s, v[1] / s);
}

inline Vector2 operator/(float s, const Vector2& v) {
	return Vector2(v[0] / s, v[1] / s);
}

inline Vector2& operator*=(Vector2& v, float s) {
	v[0] *= s;
	v[1] *= s;
	return v;
}

inline Vector2& operator/=(Vector2& v, float s) {
	v[0] /= s;
	v[1] /= s;
	return v;
}

// Vector math operations
inline float dot(const Vector2& a, const Vector2& b){
	return a[0] * b[0] + a[1] * b[1];
}

inline float squaredMagnitude(const Vector2& v){
	return dot(v, v);
}

inline float magnitude(const Vector2& v) {
	return sqrtf(squaredMagnitude(v));
}

// normalize copy
inline Vector2 normalized(const Vector2& v) {
	return v / magnitude(v);
}

// normalize in place
inline Vector2& normalize(Vector2& v) {
	return v /= magnitude(v);
}
