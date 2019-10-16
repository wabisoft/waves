// sea.hpp


inline float Sea::heightAtX(float x) const {
	float height = upper(shape).y;
	for (const Wave& wave : waves) {
		height += wave.heightAtX(x);
	}
	return height;
}

inline glm::vec2 Sea::velocityAtX(float x) const {
	glm::vec2 velocity = VEC2_ZERO;
	for (const Wave& wave : waves) {
		velocity += wave.velocityAtX(x);
	}
	return velocity;
}

inline float Sea::slopeAtX(float x) const {
	float slope = 0;
	for (const Wave& wave : waves) {
		slope += wave.slopeAtX(x);
	}
	return slope;
}
