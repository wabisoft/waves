#include "wave.hpp"

void update(Wave& wave, float deltaTime){
	//Update the wave for this time step
	wave.time += deltaTime * 2;
	if (wave.decay >= 1.f)
	{
		wave.sign = -0.25f;
	}
	else if (wave.decay <= 0.f && wave.sign <= 0)
	{
		wave.sign = 0;
		wave.decay = 0;
	}
	wave.decay = wave.decay + wave.sign * deltaTime;
	// Update velocity with timestep
	wave.velocity[0] += deltaTime;
	// Update position with velocity;
	wave.position += wave.velocity;
}

float slopeAtX(const Wave & wave, float x)
{
	// derivative of height
	return heightAtX(wave, x) * (-2 * wave.width * (x - wave.position.x) * wave.width);
}
