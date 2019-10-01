#include "sea.hpp"
#include "stage.hpp"
#include "wave.hpp"

uint8_t Sea::id_src = 0;

float heightAtX(const Sea& sea, float x) {
	float height = sea.level;
	for (const Wave& wave : sea.waves) {
		height += wave.heightAtX(x);
	}
	return height;
}

Vector2 velocityAtX(const Sea& sea, float x) {
	Vector2 velocity = VECTOR2_ZERO;
	for (const Wave& wave : sea.waves) {
		velocity += wave.velocityAtX(x);
	}
	return velocity;
}

void fixedUpdate(Sea& sea) {}


