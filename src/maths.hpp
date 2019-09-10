#pragma once

#include <cassert>
#include <cmath>
#include <vector>

/********
 * Misc *
 ********/

template <typename T>
T sign(T t) {
	return (t == (T)0) ? (T)0 : t / abs(t);
}

/***********
 * Vectors *
 ***********/

struct Vector2{
	float& operator[](int i) { return (&x)[i]; } // read
	const float& operator[](int i) const { return (&x)[i]; }; // write
	float x = 0.f, y = 0.f;
};

const Vector2 VECTOR2_UP {0.f, 1.f};
const Vector2 VECTOR2_DOWN {0.f, -1.f};
const Vector2 VECTOR2_LEFT {-1.f, 0.f};
const Vector2 VECTOR2_RIGHT {1.f, 0.f};
const Vector2 VECTOR2_ZERO {0.f, 0.f};

// Vector negation
inline Vector2 operator-(const Vector2& v);
// Vector/Vector addition and subtraction
inline Vector2 operator+(const Vector2& b, const Vector2& a);
inline Vector2 operator-(const Vector2& a, const Vector2& b);
inline Vector2& operator+=(Vector2& a, const Vector2& b);
inline Vector2& operator-=(Vector2& a, const Vector2& b);
// Vector equality an inequality
inline bool operator==(const Vector2& a, const Vector2& b);
inline bool operator!=(const Vector2& a, const Vector2& b);
// Vector/Scalar multiplication and addition
inline Vector2 operator*(const Vector2& v, float s);
inline Vector2 operator*(float s, const Vector2& v);
inline Vector2 operator/(const Vector2& v, float s);
inline Vector2 operator/(float s, const Vector2& v);
inline Vector2& operator*=(Vector2& v, float s);
inline Vector2& operator/=(Vector2& v, float s);
// Vector math operations
inline float dot(const Vector2& a, const Vector2& b);
inline float cross(const Vector2& a, const Vector2& b);
inline float squaredMagnitude(const Vector2& v);
inline float magnitude(const Vector2& v);
// normalize copy
inline Vector2 normalized(const Vector2& v);
// normalize in place
inline Vector2& normalize(Vector2& v);
inline Vector2& clamp(Vector2& v, float s); // clamp this vector to a scalar magnitude
// returns + if point on left - if point on right 0 if point on line from b to a
inline float sideSign(Vector2 a, Vector2 b, Vector2 point);
// returns true if point is bounded on the line segment defined by a and b
inline bool bounded(Vector2 a, Vector2 b, Vector2 point);

#include "vector2.inl" // definitions for template functions and inlines

/**********
 * Shapes *
 **********/

struct Circle {
	Vector2 position;
	float radius = 0.f;
};

// NOTE: Polygon vertices in the array in order from top left to topleft clockwise
// example:
//      index 0					index 1
//      	*---------------------*
//      	|                     |
//      	*---------------------*
//      index 3					 index 2
template <int N>
struct Polygon {
	Vector2 position;
	Vector2 vertices[N];
	Vector2 model[N];
	int size = N;
	float rotation = 0.f;
};

struct Rectangle : Polygon<4>{
	float width = 0.f;
	float height = 0.f;
};

inline float area(Circle & circle);
inline float area(Rectangle& rectangle);
template <int N>
inline void updateVertices(Polygon<N>& polygon);
template <int N>
inline bool pointInsidePolygon(Vector2 point, Polygon<N>& polygon);
template <int N>
inline Vector2 lower(const Polygon<N>& polygon); // gets the bottom left point of a polygon in relation to x, y axis
template <int N>
inline Vector2 upper(const Polygon<N>& polygon); // gets the top right point of a polygon in raltion to x, y axis
template <int N>
inline void boundingPoints(const Polygon<N>& polygon, Vector2& lower, Vector2& upper);


#include "shapes.inl" // definitions for template functions and inlines

Rectangle makeRectangle(Vector2 p, float w, float h, float rotation=0.f);

/************
 * Geometry *
 ************/

bool lineSegmentIntersection(Vector2 a, Vector2 b, Vector2 c, Vector2 d, Vector2& intersection);
Vector2 findNormal(Vector2 a, Vector2 b, Vector2 c);

