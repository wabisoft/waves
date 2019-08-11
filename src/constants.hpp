#pragma once

const float E =		2.71828182845904523536f; // exponential constant
const float PI =  	3.14159265358979323846f; // mmmmm pie

const float STAGE_WIDTH = 	128.f; // meters
const float STAGE_HEIGHT = 	72.f; // meters

const float FIXED_TIMESTEP = 	1.f/480.f; // seconds (120 Hz)
const float FRAME_RATE = 		1.f/120.f; // seconds (60 Hz)

const int MAX_ROCKS = 		10;
const int MAX_WAVES = 		MAX_ROCKS;
const int MAX_PLATFORMS = 	10;
const int MAX_AABBS =		MAX_ROCKS + MAX_PLATFORMS + 1; // +1 for ship and

// Physics constants
const float GRAVITATIONAL_CONSTANT = 9.8f;
const float SQUARED_TERMINAL_VELOCITY = 2.f * GRAVITATIONAL_CONSTANT * STAGE_HEIGHT;

const float WAVE_WIDTH_MULTIPLIER = 0.085f;
const float WAVE_POSITIVE_DECAY_MULTIPLIER = 5.f;
const float WAVE_NEGATIVE_DECAY_MULTIPLIER = -0.75f;
