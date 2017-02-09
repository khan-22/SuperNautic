#include "PointLight.hpp"
#include "../Log.hpp"

PointLight::PointLight(glm::vec3 position, glm::vec3 diffuseColor, GLfloat intensity)
	: _position(position), _diffuseColor(diffuseColor),
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
	return _position;
}

void PointLight::updateColor(glm::vec3 diffuseColor)
{
	_diffuseColor = diffuseColor;
}

void PointLight::changeIntensity(GLfloat intensity)
{
	_currentIntensity = intensity;
}

void PointLight::update(GLfloat dt)
{
	if (_bLightActive)
	{
		if (_transitionTimer < 1.0f)
		{
			_transitionTimer += dt / _transitionTotalTime;
			_currentIntensity = _transitionTimer * _newIntensity + (1 - _transitionTimer) * _previousIntensity;
		}
	}
}

void PointLight::transitionTo(GLfloat newIntensity, GLfloat transitionTime)
{
	_previousIntensity = _currentIntensity;
	_newIntensity = newIntensity;
	_transitionTotalTime = transitionTime;
	_transitionTimer = 0.0f;
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
		_diffuseColor,
		_currentIntensity,
		_bLightActive,
		_constant,
		_linear,
		_quadratic
	};

	return Properties;
}

const GLfloat PointLight::calculatePLBoundSize()
{
	//Defines the size of the sphere for the light
	GLfloat MaxChannel = std::fmax(std::fmax(_diffuseColor.r, _diffuseColor.g), _diffuseColor.b);
	GLfloat size = ( -_linear + glm::sqrt( glm::pow(_linear, 2) - 4 * _quadratic * (_constant - 256 * MaxChannel) * _currentIntensity )) / ( 2 * _quadratic );

	return size;
}

PointLight::~PointLight()
{

}
