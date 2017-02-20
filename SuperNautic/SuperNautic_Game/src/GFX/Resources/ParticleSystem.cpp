#include "ParticleSystem.hpp"

glm::vec3 lerp(float f, glm::vec3 a, glm::vec3 b)
{
	return (1.f - f) * a + f * b;
}

GFX::ParticleSystem::ParticleSystem()
{
}

GFX::ParticleSystem::~ParticleSystem()
{
}

void GFX::ParticleSystem::init(GLuint particleCount, glm::vec3 position, glm::vec3 velocity, GLfloat spread, GLfloat turbulence)
{
	_particles.resize(particleCount);
	
	_maxSpread	= spread;
	_turbulence = turbulence;

	update(0.f, position, velocity);
}

void GFX::ParticleSystem::update(float dt, glm::vec3 position, glm::vec3 velocity)
{
	//const float 

	//glm::vec3 turbulence = glm::normalize(glm::vec3(rand() % ));
	for (auto& particle : _particles)
	{
		particle.life -= dt;

		if (particle.life <= 0.f)
		{
			particle.pos	  = position;
			particle.velocity = -velocity;
			particle.life	  = 5.f; 
		}

		particle.pos += particle.velocity * dt;
		particle.color = lerp(particle.life / 5.f, glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 0.f, 0.f));
	}
}

void GFX::ParticleSystem::render(RenderStates & states)
{
	// Implement rendering.
	return;
}


