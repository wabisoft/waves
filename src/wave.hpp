#pragma once

#include <cstdint>
#include <vector>

#include <glm/vec2.hpp>

#include "aabb.hpp"
#include "settings.hpp"
#include "prelude.hpp"
#include "maths.hpp"
#include "util.hpp"


struct Wave {
	glm::vec2 position = {0.f, 0.f};
	glm::vec2 velocity = {0.f, 0.f};
	float amplitude = 0.f;
	float decay = 0.f;
	float time = 0.f;
	i8 direction = 1;
	i8 sign = 1;
	bool active = false;
	bool grow = true;
	u8 id = 0;

	inline float heightAtX(float x) const;
	inline glm::vec2 velocityAtX(float x) const;
	inline float slopeAtX(float x) const; // derivative of height
	inline float minimumX() const;
	inline float maximumX() const;
};

#include "wave.inl"

typedef std::vector<Wave>::iterator WaveIt;

void updateWaves(Stage& stage, Sea& sea);
u8 createWave(Sea& sea, glm::vec2 position, float amplitude, int direction, int sign);
WaveIt deleteWave(Sea& sea, WaveIt waveIt);
WaveIt deleteWave(Sea& sea, u8 waveId);
WaveIt findWave(Sea& sea, u8 waveId);
WaveIt findWaveAtPosition(Sea& sea, glm::vec2 position);
