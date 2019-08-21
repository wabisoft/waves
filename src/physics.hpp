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

// inline float impulse(Vector2 velocity, float mass1, float mass2, float restitution) {
// 	float reducedMass = (mass1 * mass2)/(mass1 + mass2);
// 	float coefficient = 1 + restitution;
// 	float velocityDifference = magnitude(velocity) - magnitude(restitution * velocity);
// 	return reducedMass * coefficient * velocityDifference;
// }

// NOTE: Don't forget to devide by the mass of the object you're using this on
inline float linearImpulse(Vector2 velocity1, Vector2 velocity2, float mass1, float mass2, float restitution) {
	float reducedMass = (mass1 * mass2)/(mass1 + mass2);
	float coefficient = 1 + restitution;
	float velocityDifference =  magnitude(velocity1) - magnitude(velocity2);
	return reducedMass * coefficient * velocityDifference;
}


// returns the magnitude of the friction of a "surface" defined by points a and b on an object with given mass
inline Vector2 friction(Vector2 a, Vector2 b, float mass, Vector2 velocity, Vector2 position) {
	Vector2 ABBA;
	// easy for unlevel surface
	// if (a.y > b.y) {
	// 	ABBA = a - b;
	// } else if (b.y > a.y){
	// 	ABBA = b - a;
	// } else {
		// need to specifically resist velocity
		float sqDistA = squaredMagnitude(a - position);
		float sqDistB = squaredMagnitude(b - position);
		Vector2 futurePos = position + velocity;
		float futureSqDistA = squaredMagnitude(a - futurePos);
		float futureSqDistB = squaredMagnitude(b - futurePos);
		if (sqDistA - futureSqDistA > 0) {
			// then we're moving toward a and friction should be from a to b
			ABBA = b - a;
		} else if (sqDistB - futureSqDistB > 0) {
			// then we're moving toward b and friction should be from b to a
			ABBA = a - b;
		} else {
			// then we're not moving and friction should be zero
			return {0.f, 0.f};
		}
	// }
	// float cosTheta = dot(ABBA, VECTOR2_RIGHT) / magnitude(ABBA);
	// float Fn = mass * GRAVITATIONAL_CONSTANT * cosTheta;  // because of our simulation technique *maybe* we don't need gravity here
	// return COEFFICIENT_OF_FRICTION * Fn * -normalized(ABBA);
	if(velocity.x < 1.f) {
	 	return COEFFICIENT_OF_FRICTION * mass * GRAVITATIONAL_CONSTANT * normalized(ABBA) * velocity.x;
	} else {
		return COEFFICIENT_OF_FRICTION * mass * GRAVITATIONAL_CONSTANT * normalized(ABBA);
	}
}
