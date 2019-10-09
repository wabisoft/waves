// wave.hpp

inline float Wave::heightAtX(float x) const {
	// get the height of the wave at x
	// Cool gaussian
	return sign * amplitude * decay * pow(E, -pow(WAVE_WIDTH_MULTIPLIER * (x - position.x), 2));
}

inline Vector2 Wave::velocityAtX(float x) const {
	float distFromMid = std::fabs(x - position.x);
	// if(distFromMid < 0) {
		// return VECTOR2_ZERO; // Don't do anything on the back of a wave
	//} else if (distFromMid == 0) {
	if (distFromMid < 0.01f) {
		return velocity * direction; // if you're at the middle of the wave get the full velocity
	} else { // else get the velocity as a ratio of your distance the center
		float max = maximumX();
		if (max < x) { return VECTOR2_ZERO; }
		float min = minimumX();
		float halfLength = (max - min)/2.f;
		return ((halfLength - distFromMid)/halfLength) * velocity * direction;
	}
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


