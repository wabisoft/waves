#pragma once

const float E =		2.71828182845904523536f; // exponential constant
const float PI =  	3.14159265358979323846; // mmmmm pie

const float STAGE_WIDTH = 	128.f; // meters
const float STAGE_HEIGHT = 	72.f; // meters

const float FIXED_TIMESTEP = 	1.f/480.f; // seconds (120 Hz)
const float FRAME_RATE = 		1.f/120.f; // seconds (60 Hz)

const int MAX_ROCKS = 		100;
const int MAX_WAVES = 		100;
const int MAX_PLATFORMS = 	100;
const int MAX_AABBS =		MAX_ROCKS + MAX_PLATFORMS + 1; // +1 for ship and 

// Physics constants
const float GRAVITATIONAL_CONSTANT = 9.8f;
const float SQUARED_TERMINAL_VELOCITY = 2 * GRAVITATIONAL_CONSTANT * STAGE_HEIGHT;

