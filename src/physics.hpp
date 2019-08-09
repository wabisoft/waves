#pragma once


#include "constants.hpp"
#include "maths.hpp"
#include "vector2.hpp"

const float TERMINAL_VELOCITY = std::sqrt(SQUARED_TERMINAL_VELOCITY);


inline Vector2 dragForce(Vector2 velocity, float fluidDensity, float mass) {
    float dragCoef = 0.5f * fluidDensity * mass;
    auto velocitySquared = Vector2(-1 * sign(velocity.x) * velocity.x * velocity.x, -1 * sign(velocity.y) * velocity.y * velocity.y);
    auto drag = dragCoef * velocitySquared;

    if (squaredMagnitude(drag) > SQUARED_TERMINAL_VELOCITY)
    {
        drag = normalized(drag) * TERMINAL_VELOCITY;
    }
    return drag;
}
