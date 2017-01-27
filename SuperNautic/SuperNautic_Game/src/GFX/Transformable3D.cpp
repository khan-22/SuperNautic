#include "Transformable3D.hpp"

#include <glm/gtx/transform.hpp>

using namespace GFX;

Transformable3D::Transformable3D()
{
}


Transformable3D::~Transformable3D()
{
}

void GFX::Transformable3D::setPosition(const glm::vec3 & position)
{
	_position = position;
	_bTransformNeedsUpdate = true;
	_bInverseTransformNeedsUpdate = true;
}

void GFX::Transformable3D::setRotation(float angle, const glm::vec3 & axis)
{
	_rotationAngle = angle;
	_rotationAxis = axis;
	_bTransformNeedsUpdate = true;
	_bInverseTransformNeedsUpdate = true;
}

void GFX::Transformable3D::setScale(const glm::vec3 & scale)
{
	_bTransformNeedsUpdate = true;
	_bInverseTransformNeedsUpdate = true;
}

void GFX::Transformable3D::setOrigin(const glm::vec3 & origin)
{
	_bTransformNeedsUpdate = true;
	_bInverseTransformNeedsUpdate = true;
}

const glm::vec3 & GFX::Transformable3D::getPosition(const glm::vec3 & position) const
{
	// TODO: insert return statement here
	return glm::vec3(0.f);
}

const glm::vec3 & GFX::Transformable3D::getScale(const glm::vec3 & scale) const
{
	// TODO: insert return statement here
	return glm::vec3(0.f);
}

const glm::vec3 & GFX::Transformable3D::getOrigin(const glm::vec3 & origin) const
{
	// TODO: insert return statement here
	return glm::vec3(0.f);
}

void GFX::Transformable3D::move(const glm::vec3 & offset)
{
}

void GFX::Transformable3D::rotate(float angle, const glm::vec3 & axis)
{
	//https://en.wikipedia.org/wiki/Rotation_formalisms_in_three_dimensions#Conversion_formulae_between_representations
}

void GFX::Transformable3D::scale(const glm::vec3 & factor)
{
}

const glm::mat4 & GFX::Transformable3D::getTransformMatrix()
{
	// TODO: insert return statement here
	// SHOULD BE CALCULATED ON NEED!!!
	return _transform;
}
