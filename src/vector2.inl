// maths.hpp
// Leave that comment up there, ycm uses it

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

/************************************************************
 * NOTE: remember that |a x b| = |a| |b| sin(theta)			*
 * that means for a 2d cross we have:                    	*
 * a x b = {												*
 * 		a.y*b.z - a.z*b.y,									*
 * 		a.z*b.x - a.x*b.z,									*
 * 		a.x*b.y - a.y*b.x									*
 * }														*
 *															*
 * in 2d the z is 0 so we get a new 3d vector				*
 * the magnitude of that vector is just the z component		*
 * That value is also |a||b|sin(theta)						*
 * which means that the sign of theta (+/-) can tell us 	*
 * if we have a counter-clockwise(-) or clockwise(+)		*
 * rotation between the 2 vectors.							*
 * this important to determine,	which side of a line a		*
 * point is on among other things							*
 ************************************************************/
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

// returns + if point on left - if point on right 0 if point on line from b to a
inline float sideSign(Vector2 a, Vector2 b, Vector2 point) {
	Vector2 u = b - a;
	Vector2 v = point - a;
	float product = cross(u, v);
	return sign(product);
}


inline bool bounded(Vector2 a, Vector2 b, Vector2 point) {
	Vector2 ba = b - a;
	if (std::abs(ba.x) > std::abs(ba.y)) { // line more horizontally oriented
		// we check that we are bounded on x
		if (ba.x > 0) { // positive dx means b.x > a.x
			return point.x >= a.x && point.x <=b.x;
		} else { // negative dx means that b.x < a.x
			return point.x >= b.x && point.x <=a.x;
		}
	} else { // line is more vertically oriented
		// we check that we are bounded on y
		if (ba.y > 0) { // positive dy means b.y > a.y
			return point.y >= a.y && point.y <= b.y;
		} else { // negative dy means that a.y > b.y
			return point.y >= b.y && point.y <= a.y;
		}
	}
	assert(false);
}
