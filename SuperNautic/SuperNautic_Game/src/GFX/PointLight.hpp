#pragma once

#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include "glm\glm.hpp"
#include "GL\glew.h"

struct PointLightProperties
{
	glm::vec3	Position;
	glm::vec3	AmbientColor;
	glm::vec3	DiffuseColor;
	GLfloat		Intensity;

	bool		bLightActive;

	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;
};

class PointLight
{
	public:
		PointLight(glm::vec3 Position, glm::vec3 AmbientColor, glm::vec3 DiffuseColor, GLfloat Intensity);

		void SetPosition(glm::vec3 Position);
		const glm::vec3 GetPosition();

		void UpdateColor(glm::vec3 AmbientColor, glm::vec3 DiffuseColor);

		void ChangeIntensity(GLfloat Intensity);
		
		void ToggleLight(bool bLightActive);

		//Returns a struct with all of the light's properties
		const PointLightProperties GetLightProperties();

		~PointLight();
	private:
		glm::vec3	_Position;
		glm::vec3	_AmbientColor;
		glm::vec3	_DiffuseColor;
		GLfloat		_Intensity;

		bool		_bLightActive;

		//Attenuation properties, should probably expose these to allow modifcation in the future
		GLfloat _Constant	= 1.0f;
		GLfloat _Linear		= 0.045f;
		GLfloat _Quadratic	= 0.0075f;

		PointLight();
};

#endif