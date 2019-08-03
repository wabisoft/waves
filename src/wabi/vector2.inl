template <typename T>
wabi::Vector2 operator*(const wabi::Vector2& v, T s) {
	return wabi::Vector2(v.x * s, v.y * s);
}

template <typename T>
wabi::Vector2 operator*(T s, const wabi::Vector2& v) {
	return wabi::Vector2(v.x * s, v.y * s);
}

template <typename T>
wabi::Vector2 operator/(const wabi::Vector2& v, T s) {
	return wabi::Vector2(v.x / s, v.y / s);
}

template <typename T>
wabi::Vector2 operator/(T s, const wabi::Vector2& v) {
	return wabi::Vector2(v.x / s, v.y / s);
}

template <typename T>
wabi::Vector2& operator*=(wabi::Vector2& v, T s) {
	v.x *= s;
	v.y *= s;
	return v;
}

template <typename T>
wabi::Vector2& operator/=(wabi::Vector2& v, T s) {
	v.x /= s;
	v.y /= s;
	return v;
}

