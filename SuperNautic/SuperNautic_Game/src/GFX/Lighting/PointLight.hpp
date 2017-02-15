#pragma once

#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include "glm\glm.hpp"
#include "GL\glew.h"

struct PointLightProperties
{
	glm::vec3	position;
	glm::vec3	diffuseColor;
	GLfloat		intensity;

	bool		bLightActive;

	GLfloat		constant;
	GLfloat		linear;
	GLfloat		quadratic;
	GLfloat		lightBoundsRadius;
};

class PointLight
{
public:
	PointLight(glm::vec3 position, glm::vec3 diffuseColor, GLfloat intensity);

	void setPosition(glm::vec3 position);
	const glm::vec3 getPosition();
	void updateColor(glm::vec3 diffuseColor);
	
	void changeIntensity(GLfloat intensity);

	void update(GLfloat dt);
	void transitionTo(GLfloat newIntensity, GLfloat transitionTime);

	void toggleLight(bool bLightActive);

	//Returns a struct with all of the light's properties
	const PointLightProperties getLightProperties();

	//Calculates the bounding sphere for the light
	void calculatePLBoundSize();

	~PointLight();
private:
	glm::vec3	_position;
	glm::vec3	_diffuseColor;

	GLfloat		_currentIntensity;
	GLfloat		_newIntensity;
	GLfloat		_previousIntensity;
	GLfloat		_transitionTimer;
	GLfloat		_transitionTotalTime;

	bool		_bLightActive;

	//Attenuation properties, should probably expose these to allow modifcation in the future
	GLfloat _constant	= 1.0f;
	GLfloat _linear		= 0.045f;
	GLfloat _quadratic	= 0.0075f;
	GLfloat _lightBoundsRadius;

	PointLight();
};

#endif