// wave.hpp

inline float Wave::heightAtX(float x) const {
	// get the height of the wave at x
	// Cool gaussian
	return sign * amplitude * decay * pow(E, -pow(WAVE_WIDTH_MULTIPLIER * (x - position.x), 2));
}

inline Vector2 Wave::velocityAtX(float x) const {
	float distFromMid = std::fabs(x - position.x);
	float max = maximumX();
	if (max < x) { return VECTOR2_ZERO; }
	float min = minimumX();
	if (min > x) { return VECTOR2_ZERO; }
	float halfLength = (max - min)/2.f;
	return ((halfLength - distFromMid)/halfLength) * velocity * direction;
}

inline float Wave::slopeAtX(float x) const {
	// derivative of height
	return heightAtX(x) * (-2 * WAVE_WIDTH_MULTIPLIER * (x - position.x) * WAVE_WIDTH_MULTIPLIER);
}

inline float Wave::minimumX() const {
	return position.x - 2.3f * (1 / WAVE_WIDTH_MULTIPLIER); // solve for heightAtX == 0
}

inline float Wave::maximumX() const {
	return position.x + 2.3f * (1 / WAVE_WIDTH_MULTIPLIER);
}


