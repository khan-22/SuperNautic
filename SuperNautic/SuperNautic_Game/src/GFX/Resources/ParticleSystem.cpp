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
	_physParticles.resize(particleCount);
	_renderParticles.resize(particleCount);
	
	_maxSpread	= spread;
	_turbulence = turbulence;
	_sizeInBytes = particleCount * sizeof(glm::vec3);

	_vao.addVertexBuffer(_sizeInBytes, GL_DYNAMIC_DRAW);
	_vao.sendDataToBuffer(0, 0, 0, _sizeInBytes, nullptr, 3, GL_FLOAT);

	update(0.f, position, velocity);
}

void GFX::ParticleSystem::update(float dt, glm::vec3 position, glm::vec3 velocity)
{
	//const float 

	//glm::vec3 turbulence = glm::normalize(glm::vec3(rand() % ));
	for (unsigned int i = 0; i < _physParticles.size(); ++i)
	{
		ParticlePhysicsData& phys = _physParticles[i];
		if (phys.life >= 0.f)
		{
			phys.life -= dt;
			phys.velocity = -velocity;
			phys.color = lerp(phys.life / 5.f, glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 0.f, 0.f));
		}

		ParticlePositionData& point = _renderParticles[i];
		point.pos += phys.velocity * dt;
	}
}

void GFX::ParticleSystem::render(RenderStates & states)
{
	// Implement rendering.
	
	// Orphaning?
	//_vao.sendDataToBuffer(0, 0, 0, _sizeInBytes, nullptr, 3, GL_FLOAT);
	_vao.sendDataToBuffer(0, 0, 0, _sizeInBytes, _renderParticles.data(), 3, GL_FLOAT);


}


