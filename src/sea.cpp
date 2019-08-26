#include "sea.hpp"
#include "stage.hpp"
#include "wave.hpp"

uint8_t Sea::id_src = 0;

float heightAtX(const Sea& sea, float x) {
	float height = sea.level;
	for (int i = 0; i < sea.numWaves; ++i) {
		height += heightAtX(sea.waves[i], x);
	}
	return height;
}

Vector2 velocityAtX(const Sea& sea, float x) {
	Vector2 velocity = VECTOR2_ZERO;
	for (int i = 0 ; i < sea.numWaves; ++i) {
		velocity += velocityAtX(sea.waves[i], x);
		// if (squaredMagnitude(velocity) > 0) {
		// 	return velocity; // return the first wave to give you a velocity
		// }
	}
	return velocity;
}

void fixedUpdate(Sea& sea) {}


