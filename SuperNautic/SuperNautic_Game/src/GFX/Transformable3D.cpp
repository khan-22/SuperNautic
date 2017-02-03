#include "Transformable3D.hpp"

#include <glm/gtx/transform.hpp>

using namespace GFX;

Transformable3D::Transformable3D() : _bTransformNeedsUpdate{false}, _bInverseTransformNeedsUpdate{false}
{
}


Transformable3D::~Transformable3D()
{
}

void GFX::Transformable3D::setPosition(const glm::vec3 & position)
{
	_position = position;
	_bTransformNeedsUpdate = true;
}

void GFX::Transformable3D::setRotation(float angle, const glm::vec3 & axis)
{
	_rotationMatrix = glm::rotate(angle, axis);

	_bTransformNeedsUpdate = true;
}

void GFX::Transformable3D::setRotation(float x, float y, float z)
{
	_rotationMatrix = glm::rotate(x, glm::vec3{1, 0, 0}) * glm::rotate(y, glm::vec3{ 0, 1, 0 }) * glm::rotate(z, glm::vec3{ 0, 0, 1 });

	_bTransformNeedsUpdate = true;
}

void GFX::Transformable3D::setScale(const glm::vec3 & scale)
{
	_scale = scale;
	_bTransformNeedsUpdate = true;
}

void GFX::Transformable3D::setScale(const float scale)
{
	_scale = { scale, scale, scale };
	_bTransformNeedsUpdate = true;
}

void GFX::Transformable3D::setOrigin(const glm::vec3 & origin)
{
	_origin = origin;
	_bTransformNeedsUpdate = true;
}

void GFX::Transformable3D::setLookAt(const glm::vec3& forwardDirection, const glm::vec3& upDirection)
{
	// Remove parts of forwardDirection not orthogonal to upDirection
	glm::vec3 forward = forwardDirection - glm::dot(forwardDirection, upDirection) * upDirection;
	forward = glm::normalize(forward);

	_lookAt = glm::lookAtRH(glm::vec3{ 0,0,0 }, forward, upDirection);

	_bTransformNeedsUpdate = true;
}

const glm::vec3 & GFX::Transformable3D::getPosition() const
{
	return _position;
}

const glm::mat4& GFX::Transformable3D::getRotation() const
{
	return _rotationMatrix;
}

const glm::mat4& GFX::Transformable3D::getLookAt() const
{
	return _lookAt;
}

const glm::vec3 & GFX::Transformable3D::getScale() const
{
	return _scale;
}

const glm::vec3 & GFX::Transformable3D::getOrigin() const
{
	return _origin;
}

void GFX::Transformable3D::move(const glm::vec3 & offset)
{
	_position += offset;
	_bTransformNeedsUpdate = true;
}

void GFX::Transformable3D::rotate(float angle, const glm::vec3 & axis)
{
	_rotationMatrix = glm::rotate(_rotationMatrix, angle, axis);

	_bTransformNeedsUpdate = true;
}

void GFX::Transformable3D::rotate(float x, float y, float z)
{
	_rotationMatrix = glm::rotate(z, glm::vec3{ 0, 0, 1 }) * glm::rotate(y, glm::vec3{ 0, 1, 0 }) * glm::rotate(_rotationMatrix, x, glm::vec3{ 1, 0, 0 });

	_bTransformNeedsUpdate = true;
}

void GFX::Transformable3D::scale(const glm::vec3 & factor)
{
	_scale *= factor;

	_bTransformNeedsUpdate = true;
}

void GFX::Transformable3D::scale(const float factor)
{
	_scale *= factor;

	_bTransformNeedsUpdate = true;
}

const glm::mat4 & GFX::Transformable3D::getTransformMatrix()
{
	if (_bTransformNeedsUpdate)
	{
		_transform = glm::translate(_position) * _lookAt * _rotationMatrix * glm::scale(_scale) * glm::translate(-_origin);
		_bInverseTransformNeedsUpdate = true;
		_bTransformNeedsUpdate = false;
	}
	return _transform;
}

const glm::mat4 & GFX::Transformable3D::getInverseTransformMatrix()
{
	if (_bInverseTransformNeedsUpdate)
	{
		_inverseTransform = glm::inverse(getTransformMatrix());
		_bInverseTransformNeedsUpdate = false;
	}
	return _inverseTransform;
}