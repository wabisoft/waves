#include "sea.hpp"
#include "stage.hpp"
#include "wave.hpp"


float heightAtX(const Sea& sea, float x) {
	auto height = sea.level;
	for (auto &&wave : sea.waves) {
		height += heightAtX(wave, x);
	}
	return height;
}

void fixedUpdate(Sea& sea) {}

