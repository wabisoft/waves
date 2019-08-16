#pragma once

#include "constants.hpp"
#include "maths.hpp"

const Vector2 GRAVITY = {0.f, -GRAVITATIONAL_CONSTANT};
const Vector2 GRAVITY_PER_FRAME = {0.f, -GRAVITATIONAL_CONSTANT * FIXED_TIMESTEP};
const float TERMINAL_VELOCITY = std::sqrt(SQUARED_TERMINAL_VELOCITY);


inline Vector2 dragForce(Vector2 velocity, float fluidDensity, float mass) {
    float dragCoef = 0.5f * fluidDensity * mass;
    Vector2 velocitySquared = {-1 * sign(velocity.x) * velocity.x * velocity.x, -1 * sign(velocity.y) * velocity.y * velocity.y};
    auto drag = dragCoef * velocitySquared;

    if (squaredMagnitude(drag) > SQUARED_TERMINAL_VELOCITY)
    {
        drag = normalized(drag) * TERMINAL_VELOCITY;
    }
    return drag;
}

inline float impulse(Vector2 velocity, float mass1, float mass2, float restitution) {
	float reducedMass = (mass1 * mass2)/(mass1 + mass2);
	float coefficient = 1 + restitution;
	float velocityDifference = magnitude(velocity) - magnitude(restitution * velocity);
	return reducedMass * coefficient * velocityDifference;
}
