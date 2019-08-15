/*****************
 * 	Vector Stuff *
 *****************/

// Vector negation
inline Vector2 operator-(const Vector2& v) {
	return {-v[0], -v[1]};
}

// Vector/Vector addition and subtraction
inline Vector2 operator+(const Vector2& b, const Vector2& a) {
	return {a[0] + b[0], a[1] + b[1]};

}

inline Vector2 operator-(const Vector2& a, const Vector2& b) {
	return {a[0] - b[0], a[1] - b[1]};
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
	return {v[0] * s, v[1] * s};
}

inline Vector2 operator*(float s, const Vector2& v) {
	return {v[0] * s, v[1] * s};
}

inline Vector2 operator/(const Vector2& v, float s) {
	return {v[0] / s, v[1] / s};
}

inline Vector2 operator/(float s, const Vector2& v) {
	return {v[0] / s, v[1] / s};
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
inline float dot(const Vector2& a, const Vector2& b) {
	return a[0] * b[0] + a[1] * b[1];
}

inline float cross(const Vector2& a, const Vector2&b) {
	return a[0]*b[1] - a[1]*b[0]; // the z component of the 3 dimensional cross
}

inline float squaredMagnitude(const Vector2& v) {
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


