#pragma once

#include <limits>

const float E =		2.71828182845904523536f; // exponential constant
const float PI =  	3.14159265358979323846f; // mmmmm pie
const float INF = 	std::numeric_limits<float>::infinity();

const float STAGE_WIDTH = 	128.f; // meters
const float STAGE_HEIGHT = 	72.f; // meters

const float FIXED_TIMESTEP = 	1.f/480.f; // seconds (480 Hz)
const float FRAME_RATE = 		1.f/120.f; // seconds (60 Hz)

const float WOOD_DENSITY = 112.f;
const float WATER_DENSITY = 998.f;
const float AIR_DENSITY = 1.225f;


// Physics constants
// const float GRAVITATIONAL_CONSTANT = 9.8f;
const float GRAVITATIONAL_CONSTANT = 5.8f;
const float SQUARED_TERMINAL_VELOCITY = 2.f * GRAVITATIONAL_CONSTANT * STAGE_HEIGHT;

const float WAVE_WIDTH_MULTIPLIER = 0.085f;
const float WAVE_POSITIVE_DECAY_MULTIPLIER = 5.f;
const float WAVE_NEGATIVE_DECAY_MULTIPLIER = -0.75f;
const float ROLLING_RESISTANCE_COEFFICIENT = 0.03f; // from a table on this page https://www.school-for-champions.com/science/friction_rolling_coefficient.htm#.XVtDwHspCUk
const float COEFFICIENT_OF_FRICTION = 2.f;

/******************
 * MAGIC NUMBERS! *
 ******************/

// Magic Rocks
const float ROCK_RADIUS_MASS_RATIO = 2.f/3.f; // i.e if rock radius is 3 then mass should be 2;
const float ROCK_MIN_RADIUS = 1.f;
const float ROCK_MAX_RADIUS = 4.f;
const float ROCK_MIN_RADIUS_SQUARED = ROCK_MIN_RADIUS * ROCK_MIN_RADIUS;
const float ROCK_MAX_RADIUS_SQUARED = ROCK_MAX_RADIUS * ROCK_MAX_RADIUS;
const float ROCK_START_RADIUS = ROCK_MIN_RADIUS;
const float ROCK_MAX_SPEED = 1.75f; // meters/second (remember to multiply by timestep)
const float ROCK_RESTITUTION = 0.00001f; // FIXME: these numbers don't make sense but they look/feel right, diagnse
const float ROCK_MAX_FLOAT_TIME = 2.f;

// Magic Platforms
const float PLATFORM_AREA_MASS_RATIO = 2.f/3.f;

// Magic Ships
const float SHIP_AREA_MASS_RATIO = 2.f/3.f;
const float SHIP_RESTITUTION = 0.00001f;
const float SHIP_MAX_SPEED = 1.75f; // meters/second (remember to multiply by timestep)

// Magic numbers for STAGE
const float STAGE_MAX_PULL_LENGTH = 8.f;
const float STAGE_MAX_PULL_LENGTH_SQUARED = STAGE_MAX_PULL_LENGTH * STAGE_MAX_PULL_LENGTH;


const char * const GAME_TITLE = "Waves!!!!";
