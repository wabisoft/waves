#pragma once

#define WABI_NOT_USED(x) ((void)(x))

// types
typedef signed char	i8;
typedef signed short i16;
typedef signed int i32;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;
typedef double f64;

#ifndef _WIN32
// #define FLT_MAX  __FLT_MAX__;
// #define FLT_EPSILON  __FLT_EPSILON__;
#include <cfloat>
#endif
// constants
const f32 wabi_maxFloat	= FLT_MAX;
const f32 wabi_epsilon = FLT_EPSILON;
const f32 E =	2.71828182845904523536f; // exponential constant
constexpr f32 PI = 3.14159265358979323846f; // mmmmm pie
#include <limits>
const f32 INF = std::numeric_limits<float>::infinity();


// magic numbers and strings
const f32 STAGE_WIDTH = 128.f; // meters
const f32 STAGE_HEIGHT = 72.f; // meters

const f32 FPS = 120.f; // frames/second
const f32 UPS = 480.f; // updates/second

const f32 FRAME_RATE = 1.f / FPS;
const f32 FIXED_TIMESTEP = 1.f/UPS;

// Physics constants

// Density
const f32 WOOD_DENSITY = 112.f;
const f32 WATER_DENSITY = 998.f;
const f32 AIR_DENSITY = 1.225f;

const f32 GRAVITATIONAL_CONSTANT = 5.8f;
const f32 SQUARED_TERMINAL_VELOCITY = 2.f * GRAVITATIONAL_CONSTANT * STAGE_HEIGHT;

const f32 WAVE_WIDTH_MULTIPLIER = 0.085f;
const f32 WAVE_POSITIVE_DECAY_MULTIPLIER = 5.f;
const f32 WAVE_NEGATIVE_DECAY_MULTIPLIER = -0.75f;
const f32 ROLLING_RESISTANCE_COEFFICIENT = 0.03f; // from a table on this page https://www.school-for-champions.com/science/friction_rolling_coefficient.htm#.XVtDwHspCUk
const f32 COEFFICIENT_OF_FRICTION = 2.f;

/******************
 * MAGIC NUMBERS! *
 ******************/

// Magic Rocks
const f32 ROCK_RADIUS_MASS_RATIO = 2.f/3.f; // i.e if rock radius is 3 then mass should be 2;
const f32 ROCK_MIN_RADIUS = 1.f;
const f32 ROCK_MAX_RADIUS = 4.f;
const f32 ROCK_MIN_RADIUS_SQUARED = ROCK_MIN_RADIUS * ROCK_MIN_RADIUS;
const f32 ROCK_MAX_RADIUS_SQUARED = ROCK_MAX_RADIUS * ROCK_MAX_RADIUS;
const f32 ROCK_START_RADIUS = ROCK_MIN_RADIUS;
const f32 ROCK_MAX_SPEED = 1.75f; // meters/second (remember to multiply by timestep)
const f32 ROCK_RESTITUTION = 0.00001f; // FIXME: these numbers don't make sense but they look/feel right, diagnse
const f32 ROCK_MAX_FLOAT_TIME = 2.f;

// Magic Platforms
const f32 PLATFORM_AREA_MASS_RATIO = 2.f/3.f;

// Magic Ships
const f32 SHIP_AREA_MASS_RATIO = 2.f/3.f;
const f32 SHIP_RESTITUTION = 0.00001f;
const f32 SHIP_MAX_SPEED = 1.75f; // meters/second (remember to multiply by timestep)

// Magic numbers for STAGE
const f32 STAGE_MAX_PULL_LENGTH = 8.f;
const f32 STAGE_MAX_PULL_LENGTH_SQUARED = STAGE_MAX_PULL_LENGTH * STAGE_MAX_PULL_LENGTH;


const char * const GAME_TITLE = "Waves!!!!";
