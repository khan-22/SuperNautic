#pragma once

#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include "glm\glm.hpp"
#include "GL\glew.h"

class PointLight
{
	public:
		PointLight(glm::vec3 Position, glm::vec3 AmbientColor, glm::vec3 DiffuseColor, GLfloat Intensity);

		void SetPosition(glm::vec3 Position);
		const glm::vec3 GetPosition();

		void UpdateColor(glm::vec3 AmbientColor, glm::vec3 DiffuseColor);

		void ChangeIntensity(GLfloat Intensity);
		
		void ToggleLight(bool bLightActive);

	private:
		glm::vec3	_Position;
		glm::vec3	_AmbientColor;
		glm::vec3	_DiffuseColor;
		GLfloat		_Intensity;

		bool		_bLightActive;

		//Attenuation properties, should probably expose these to allow modifcation in the future
		GLfloat constant	= 1.0f;
		GLfloat linear		= 0.045f;
		GLfloat quadratic	= 0.0075f;

		PointLight();
		~PointLight();
};

#endif