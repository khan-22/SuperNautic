#include "GFX/Lighting/DirLight.h"

DirLight::DirLight(glm::vec3 direction, glm::vec3 ambientColor, glm::vec3 diffuseColor, GLfloat intensity)
	: _direction(direction), _ambientColor(ambientColor), _diffuseColor(diffuseColor), _currentIntensity(intensity)
{

}

void DirLight::update(GLfloat dt)
{
	if (_transitionTimer < 1.0f)
	{
		_transitionTimer += dt / _transitionTotalTime;
		_currentIntensity = _transitionTimer * _newIntensity + (1 - _transitionTimer) * _previousIntensity;
	}
}

void DirLight::changeIntensity(GLfloat newIntensity, GLfloat transitionTime)
{
	_previousIntensity = _currentIntensity;
	_newIntensity = newIntensity;
	_transitionTotalTime = transitionTime;
	_transitionTimer = 0.0f;
}

const DirLightProperties DirLight::getLightProperties()
{
	DirLightProperties Properties
	{
		_direction,
		_ambientColor,
		_diffuseColor,
		_currentIntensity
	};

	return Properties;
}
