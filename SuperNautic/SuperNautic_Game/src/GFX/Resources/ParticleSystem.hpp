#pragma once

#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

#include "GFX/Resources/VertexArrayObject.hpp"
#include "GFX/Rendering/Renderable3D.hpp"

namespace GFX
{

	class ParticleSystem : Renderable3D
	{
	public:
		ParticleSystem();
		~ParticleSystem();

		void init(GLuint particleCount, glm::vec3 position, glm::vec3 velocity, GLfloat spread, GLfloat turbulence);

		void update(float dt, glm::vec3 position, glm::vec3 velocity);

		void render(RenderStates& states) override;

	protected:

	private:
		struct ParticlePositionData
		{
			glm::vec3 pos = glm::vec3(0.f);
		};

		struct ParticlePhysicsData
		{
			glm::vec3 velocity  = glm::vec3(0.f);
			glm::vec3 color		= glm::vec3(1.f);
			float	  life		= 0.f;
		};

		std::vector<ParticlePhysicsData>	_physParticles;
		std::vector<ParticlePositionData>	_renderParticles;

		GLfloat		_turbulence;
		GLfloat		_maxSpread;

		GLsizei		_sizeInBytes;
		
		VertexArrayObject _vao;
	};

}
#endif // PARTICLE_SYSTEM_HPP

