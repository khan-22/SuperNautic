#include "PointLight.hpp"
#include "../Log.hpp"

PointLight::PointLight(glm::vec3 position, glm::vec3 ambientColor, glm::vec3 diffuseColor, GLfloat intensity)
	: _position(position), _ambientColor(ambientColor), _diffuseColor(diffuseColor),
	_currentIntensity(intensity)
{
	_bLightActive = true;
}

void PointLight::setPosition(glm::vec3 position)
{
	_position = position;
}

const glm::vec3 PointLight::getPosition()
{
	return glm::vec3();
}

void PointLight::updateColor(glm::vec3 ambientColor, glm::vec3 diffuseColor)
{
	_ambientColor = ambientColor;
	_diffuseColor = diffuseColor;
}

void PointLight::changeIntensity(GLfloat intensity)
{
	_currentIntensity = intensity;
}

void PointLight::transitionTo(GLfloat newIntensity, GLfloat transitionTime)
{
	_previousIntensity = _currentIntensity;
	_newIntensity = newIntensity;
	_transitionTotalTime = transitionTime;
	_transitionTimer = 0.0f;
}

void PointLight::update(GLfloat dt)
{
	if (_transitionTimer < 1.0f)
	{
		_transitionTimer += dt / _transitionTotalTime;
		_currentIntensity = _transitionTimer * _newIntensity + (1 - _transitionTimer) * _previousIntensity;
	}
	
	LOG("Current intensity: ", _currentIntensity);
}


void PointLight::toggleLight(bool bLightActive)
{
	_bLightActive = bLightActive;
}

const PointLightProperties PointLight::getLightProperties()
{
	PointLightProperties Properties
	{
		_position,
		_ambientColor,
		_diffuseColor,
		_currentIntensity,
		_bLightActive,
		_constant,
		_linear,
		_quadratic
	};

	return Properties;
}

PointLight::~PointLight()
{

}
