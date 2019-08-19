/*****************
 * 	Vector Stuff *
 *****************/

// Vector negation
inline Vector2 operator-(const Vector2& v) {
	return {-v.x, -v.y};
}

// Vector/Vector addition and subtraction
inline Vector2 operator+(const Vector2& b, const Vector2& a) {
	return {a.x + b.x, a.y + b.y};

}

inline Vector2 operator-(const Vector2& a, const Vector2& b) {
	return {a.x - b.x, a.y - b.y};
}

inline Vector2& operator+=(Vector2& a, const Vector2& b) {
	a.x += b.x;
	a.y += b.y;
	return a;
}

inline Vector2& operator-=(Vector2& a, const Vector2& b) {
	a.x -= b.x;
	a.x -= b.y;
	return a;
}

// Vector equality an inequality
inline bool operator==(const Vector2& a, const Vector2& b) {
	return a.x == b.x && a.y == b.y;
}

inline bool operator!=(const Vector2& a, const Vector2& b) {
	return a.x != b.x || a.y != b.y;
}


// Vector/Scalar multiplication and addition
inline Vector2 operator*(const Vector2& v, float s) {
	return {v.x * s, v.y * s};
}

inline Vector2 operator*(float s, const Vector2& v) {
	return {v.x * s, v.y * s};
}

inline Vector2 operator/(const Vector2& v, float s) {
	return {v.x / s, v.y / s};
}

inline Vector2 operator/(float s, const Vector2& v) {
	return {v.x / s, v.y / s};
}

inline Vector2& operator*=(Vector2& v, float s) {
	v.x *= s;
	v.y *= s;
	return v;
}

inline Vector2& operator/=(Vector2& v, float s) {
	v.x /= s;
	v.y /= s;
	return v;
}

// Vector math operations
inline float dot(const Vector2& a, const Vector2& b) {
	return a.x * b.x + a.y * b.y;
}

inline float cross(const Vector2& a, const Vector2&b) {
	return a.x*b.y - a.y*b.x; // the z component of the 3 dimensional cross
}

inline float squaredMagnitude(const Vector2& v) {
	return dot(v, v);
}

inline float magnitude(const Vector2& v) {
	return sqrtf(dot(v,v));
}

// normalize copy
inline Vector2 normalized(const Vector2& v) {
	return v / magnitude(v);
}

// normalize in place
inline Vector2& normalize(Vector2& v) {
	return v /= magnitude(v);
}

inline Vector2& clamp(Vector2& v, float s) {
	float mag = magnitude(v);
	if (mag > s) {
		v /= mag;
		v *= s;
	}
	return v;
}
