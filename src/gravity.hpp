#pragma once

#include "constants.hpp"
#include "vector2.hpp"
// #include "ship.hpp"


const Vector2 GRAVITY = Vector2(0.f, -GRAVITATIONAL_CONSTANT);
const Vector2 GRAVITY_PER_FRAME = Vector2(0.f, -GRAVITATIONAL_CONSTANT * FIXED_TIMESTEP);

// inline void applyGravity(const Ship& ship) { ship.velocity += GRAVITY; }
// inline void applyGravity(const Rock& rock) { rock.velocity += GRAVITY; }
