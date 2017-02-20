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

	class ParticleSystem : public Renderable3D
	{
	public:
		ParticleSystem();
		~ParticleSystem();

		void init(GLuint particleCount, glm::vec3 position, glm::vec3 velocity, GLfloat maxLifetime, GLfloat spread, GLfloat turbulence);

		void update(float dt, glm::vec3 position, glm::vec3 velocity);

		void render(RenderStates& states) override;

	protected:

	private:
		struct ParticleRenderData
		{
			std::vector<glm::vec3>	positions;
			std::vector<glm::vec3>	colors;
			std::vector<GLfloat>	sizes;
		};

		struct ParticlePhysicsData
		{
			glm::vec3 velocity  = glm::vec3(0.f);
			//glm::vec3 color		= glm::vec3(1.f);
			double	  life		= 0.f;
		};

		std::vector<ParticlePhysicsData>	_physParticles;
		ParticleRenderData	_renderParticles;

		GLuint		_particleCount;
		GLfloat		_turbulence;
		glm::vec3	_previousTurbulence;
		GLfloat		_maxSpread;
		GLfloat		_maxLifetime;

		GLsizei		_sizeInBytes;
		GLsizei		_positionsSizeInBytes;
		GLsizei		_colorsSizeInBytes;
		GLsizei		_sizesSizeInBytes;
		
		std::unique_ptr<VertexArrayObject> _vao;
	};

}
#endif // PARTICLE_SYSTEM_HPP

