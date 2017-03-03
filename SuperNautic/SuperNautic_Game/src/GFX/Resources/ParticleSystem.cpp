#include "ParticleSystem.hpp"


glm::vec3 lerp(float f, glm::vec3 a, glm::vec3 b)
{
	return (1.f - f) * a + f * b;
}


GLfloat lerp(float f, GLfloat a, GLfloat b)
{
	return (1.f - f) * a + f * b;
}

glm::vec3 randomVector()
{
	glm::vec3 v = glm::vec3(
		(rand() % 100 - 50) / 100.f, 
		(rand() % 100 - 50) / 100.f, 
		(rand() % 100 - 50) / 100.f);

	return glm::normalize(v);
}

GFX::ParticleSystem::ParticleSystem()
	: _bIsRunning(false)
	, _birthColor(glm::vec3(1.f, 1.f, 0.f))
	, _deathColor(glm::vec3(1.f, 0.f, 0.f))
	, _birthSize(0.5f)
	, _deathSize(0.f)
{
}

GFX::ParticleSystem::~ParticleSystem()
{
}

void GFX::ParticleSystem::init(GLuint particleCount, glm::vec3 position, glm::vec3 velocity, GLfloat maxLifetime, GLfloat spread, GLfloat turbulence)
{
	_vao.reset(new VertexArrayObject);

	_physParticles.resize(particleCount);

	_renderParticles.positions.resize(particleCount);
	_renderParticles.colors.resize(particleCount);
	_renderParticles.sizes.resize(particleCount);
	
	_particleCount	  = particleCount;
	_maxLifetime	  = maxLifetime;
	_maxSpread		  = spread;
	_turbulence		  = turbulence;
	_previousPosition = position;

	_positionsSizeInBytes = particleCount * sizeof(glm::vec3);
	_colorsSizeInBytes	  = particleCount * sizeof(glm::vec3);
	_sizesSizeInBytes	  = particleCount * sizeof(GLfloat);
	_sizeInBytes		  = _positionsSizeInBytes + _sizesSizeInBytes + _colorsSizeInBytes;

	_previousTurbulence = (randomVector() * _turbulence) * 0.1f;

	_vao->addVertexBuffer(_sizeInBytes, GL_DYNAMIC_DRAW);

	GLsizei offset = 0;
	_vao->sendDataToBuffer(0, 0, offset, _positionsSizeInBytes, nullptr, 3, GL_FLOAT);
	offset += _positionsSizeInBytes;
	_vao->sendDataToBuffer(0, 1, offset, _colorsSizeInBytes, nullptr, 3, GL_FLOAT);
	offset += _colorsSizeInBytes;
	_vao->sendDataToBuffer(0, 2, offset, _sizesSizeInBytes, nullptr, 1, GL_FLOAT);
	offset += _sizesSizeInBytes;

	_vao->setDrawCount(particleCount);

	for (unsigned int i = 0; i < _particleCount; ++i)
	{
		ParticlePhysicsData& phys = _physParticles[i];
		phys.life = -1.f;
	}

	update(0.f, position, glm::vec3{0,0,0});

	_texture = TextureCache::get("particle01.png");
}

void GFX::ParticleSystem::update(float dt, glm::vec3 position, glm::vec3 startVelocity)
{
	//const float 

	glm::vec3 turbulence = (randomVector() * _turbulence * 0.1f) + _previousTurbulence * 0.9f;
	
	if (_bIsRunning)
	{
		for (unsigned int i = 0; i < _particleCount; ++i)
		{
			ParticlePhysicsData& phys = _physParticles[i];

			if (phys.life >= 0.f)
			{
				phys.life -= dt;

				glm::vec3 relativeVelocity = phys.velocity - turbulence;

				phys.velocity -= 0.5f * relativeVelocity * dt;
				//phys.color = lerp(phys.life / 5.f, glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 0.f, 0.f));

				_renderParticles.positions[i] += (phys.velocity) * dt;
			}
			else
			{
				glm::vec3 spread = randomVector() * _maxSpread * (float)(rand() % 10) * 0.1f;

				glm::vec3 spawnInterval = (_previousPosition - position);
				phys.velocity = startVelocity + spread;

				double randomFactor = double(rand() % (int)(_particleCount)) / (double)(_particleCount);
				phys.life = _maxLifetime * (0.8f + 0.3f * randomFactor);

				double randomFactor2 = double(rand() % (int)(_particleCount)) / (double)(_particleCount);
				float rf2 = (float)randomFactor2;

				//phys.color = lerp(phys.life / 5.f, glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 0.f, 0.f));

				_renderParticles.positions[i] = position + spawnInterval * (float)randomFactor;
				_renderParticles.positions[i] += (phys.velocity * (float)randomFactor2) * dt;
			}

			
			_renderParticles.colors[i] = lerp(phys.life / _maxLifetime, _deathColor, _birthColor);
			_renderParticles.sizes[i] = lerp(phys.life / _maxLifetime, _deathSize, _birthSize);
		}
	}
	else
	{
		for (unsigned int i = 0; i < _particleCount; ++i)
		{
			ParticlePhysicsData& phys = _physParticles[i];
			if (phys.life >= 0.f)
			{
				phys.life -= dt;
				glm::vec3 relativeVelocity = phys.velocity - turbulence;
				phys.velocity -= 0.5f * glm::dot(relativeVelocity, relativeVelocity) * glm::normalize(relativeVelocity) * dt;
				
				_renderParticles.sizes[i] = lerp(phys.life / _maxLifetime, _deathSize, _birthSize);
			}
			else
			{
				_renderParticles.sizes[i] = 0.f;
			}

			_renderParticles.positions[i] += (phys.velocity) * dt;
			_renderParticles.colors[i] = lerp(phys.life / _maxLifetime, _deathColor, _birthColor);
		}
	}
	

	_previousPosition = position;
}

void GFX::ParticleSystem::render(RenderStates & states)
{
	// Implement rendering.
	
	// Orphaning?
	//_vao.sendDataToBuffer(0, 0, 0, _sizeInBytes, nullptr, 3, GL_FLOAT);
	GLsizei offset = 0;
	_vao->updateDataInBuffer(0, 0, offset, _positionsSizeInBytes, _renderParticles.positions.data());
	offset += _positionsSizeInBytes;
	_vao->updateDataInBuffer(0, 1, offset, _colorsSizeInBytes, _renderParticles.colors.data());
	offset += _colorsSizeInBytes;
	_vao->updateDataInBuffer(0, 2, offset, _sizesSizeInBytes, _renderParticles.sizes.data());
	offset += _sizesSizeInBytes;

	_texture.get()->bind(0);
	_vao->renderPoints();
	_texture.get()->unbind(0);

}

void GFX::ParticleSystem::start()
{
	_bIsRunning = true;
}

void GFX::ParticleSystem::stop()
{
	_bIsRunning = false;
}

void GFX::ParticleSystem::setBirthColor(glm::vec3 color)
{
	_birthColor = color;
}

void GFX::ParticleSystem::setDeathColor(glm::vec3 color)
{
	_deathColor = color;
}

void GFX::ParticleSystem::setBirthSize(float size)
{
	_birthSize = size;
}

void GFX::ParticleSystem::setDeathSize(float size)
{
	_deathSize = size;
}

void GFX::ParticleSystem::setLifeTime(float lifeTime)
{
	_maxLifetime = lifeTime;
}


