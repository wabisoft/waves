#include "sea.hpp"
#include "stage.hpp"
#include "wave.hpp"

uint8_t Sea::id_src = 0;

float heightAtX(const Sea& sea, float x) {
	float height = sea.level;
	for (short i = 0; i < sea.numWaves; ++i) {
		height += heightAtX(sea.waves[i], x);
	}
	return height;
}

void fixedUpdate(Sea& sea) {}


