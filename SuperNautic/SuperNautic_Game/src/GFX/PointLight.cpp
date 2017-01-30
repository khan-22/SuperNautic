#include "PointLight.hpp"

PointLight::PointLight(glm::vec3 Position, glm::vec3 AmbientColor, glm::vec3 DiffuseColor, GLfloat Intensity)
	: _Position(Position), _AmbientColor(AmbientColor), _DiffuseColor(DiffuseColor),
	_Intensity(Intensity)
{

}

void PointLight::SetPosition(glm::vec3 Position)
{
	_Position = Position;
}

const glm::vec3 PointLight::GetPosition()
{
	return glm::vec3();
}

void PointLight::UpdateColor(glm::vec3 AmbientColor, glm::vec3 DiffuseColor)
{
	_AmbientColor = AmbientColor;
	_DiffuseColor = DiffuseColor;
}

void PointLight::ChangeIntensity(GLfloat Intensity)
{
	_Intensity = Intensity;
}

void PointLight::ToggleLight(bool bLightActive)
{
	_bLightActive = bLightActive;
}

const PointLightProperties PointLight::GetLightProperties()
{
	PointLightProperties Properties
	{
		_Position,
		_AmbientColor,
		_DiffuseColor,
		_Intensity,
		_bLightActive,
		_Constant,
		_Linear,
		_Quadratic
	};

	return Properties;
}

PointLight::~PointLight()
{

}
