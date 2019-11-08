#pragma once

#include <glm/vec2.hpp>

#include "settings.hpp"
#include "maths.hpp"
#include "shapes.hpp"

const glm::vec2 GRAVITY = {0.f, -GRAVITATIONAL_CONSTANT};
const glm::vec2 GRAVITY_PER_FRAME = {0.f, -GRAVITATIONAL_CONSTANT * FIXED_TIMESTEP};
const float TERMINAL_VELOCITY = std::sqrt(SQUARED_TERMINAL_VELOCITY);

// TODO: revisit the idea of encapsulating physics data into a body class
struct Body {
	glm::vec2 velocity = glm::vec2(0);
	glm::vec2 acceleration = glm::vec2(0);
	wabi::Polygon* pShape = nullptr;
	glm::vec2* position = nullptr;
};


inline glm::vec2 dragForce(glm::vec2 velocity, float fluidDensity, float mass) {
    float dragCoef = 0.5f * fluidDensity * mass;
    glm::vec2 velocitySquared = {-1 * glm::sign(velocity.x) * velocity.x * velocity.x, -1 * glm::sign(velocity.y) * velocity.y * velocity.y};
    auto drag = dragCoef * velocitySquared;

    if (glm::dot(drag, drag) > SQUARED_TERMINAL_VELOCITY)
    {
        drag = glm::normalize(drag) * TERMINAL_VELOCITY;
    }
    return drag;
}

// NOTE: Don't forget to devide by the mass of the object you're using this on
inline float linearImpulse(glm::vec2 velocity1, glm::vec2 velocity2, float mass1, float mass2, float restitution) {
	float reducedMass = (mass1 * mass2)/(mass1 + mass2);
	float coefficient = 1 + restitution;
	float velocityDifference =  glm::length(velocity1) - glm::length(velocity2);
	return reducedMass * coefficient * velocityDifference;
}


// returns the magnitude of the friction of a "surface" defined by points a and b on an object with given mass
inline glm::vec2 friction(glm::vec2 a, glm::vec2 b, float mass, glm::vec2 velocity, glm::vec2 position) {
	glm::vec2 ABBA;
	// easy for unlevel surface
	if (a.y > b.y) {
		ABBA = a - b;
	} else if (b.y > a.y){
		ABBA = b - a;
	} else {
		// need to specifically resist velocity
		glm::vec2 relA = a - position;
		glm::vec2 relB = b - position;
		float sqDistA = dot(relA, relA);
		float sqDistB = dot(relB, relB);
		glm::vec2 futurePos = position + velocity;
		glm::vec2 relAFuture = a - futurePos;
		glm::vec2 relBFuture = b - futurePos;
		float futureSqDistA = glm::dot(relAFuture, relBFuture);
		float futureSqDistB = glm::dot(relBFuture, relBFuture);
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
	}
	// float cosTheta = dot(ABBA, VECTOR2_RIGHT) / magnitude(ABBA);
	// float Fn = mass * GRAVITATIONAL_CONSTANT * cosTheta;  // because of our simulation technique *maybe* we don't need gravity here
	// return COEFFICIENT_OF_FRICTION * Fn * -normalized(ABBA);
	// if(velocity.x < 1.f) {
	//  	return COEFFICIENT_OF_FRICTION * mass * GRAVITATIONAL_CONSTANT * -normalized(ABBA) * velocity.x;
	// } else {
	return COEFFICIENT_OF_FRICTION * mass * GRAVITATIONAL_CONSTANT * - glm::normalize(ABBA);
	// }
}
