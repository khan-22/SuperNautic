#include "SpringRotatedVector.hpp"
#include "Utilities.hpp"
#include "glm/gtx/vector_angle.hpp"

SpringRotatedVector::SpringRotatedVector(const glm::vec3& vector, const glm::vec3& target, const glm::vec3& backupAxis, float springConstant, float dampingConstant)
	: _vector{ glm::normalize(vector) }, _target{ glm::normalize(target) }, _backupAxis{ glm::normalize(backupAxis) }, _springConstant { springConstant }, _dampingConstant{ dampingConstant }
{

}

void SpringRotatedVector::update(float dt)
{
	glm::vec3 newAxis = glm::normalize(glm::cross(_vector, _target));

	// Find angle between vectors
	float angleBetween;
	float dot = glm::dot(_vector, _target);
	if (dot >= 0.999f)
	{
		angleBetween = 0.0f;
		newAxis = _backupAxis;
	}
	else if (dot <= -0.999f)
	{
		angleBetween = glm::pi<float>();
		newAxis = _backupAxis;
	}
	else
	{
		angleBetween = acosf(dot);
	}

	if (!isnan(newAxis.x))
	{
		// Update rotation axis and velocity (represented by length)
		_axis += glm::normalize(newAxis) * angleBetween * angleBetween * _springConstant * dt;
	}

	// Reduce velocity (represented by axis length) using damping constant
	_axis -= _axis * _dampingConstant * dt;

	// Rotate vector
	if (!bAlmostEqual(_axis, glm::vec3{ 0, 0, 0 }))
	{
		glm::mat4 rotation = glm::rotate(glm::length(_axis) * dt, glm::normalize(_axis));

		_vector = rotation * glm::vec4{ _vector, 0.0f };
	}
}

const glm::vec3& SpringRotatedVector::operator()() const
{
	return _vector;
}

const glm::vec3& SpringRotatedVector::getVector() const
{
	return (*this)();
}

void SpringRotatedVector::setTarget(const glm::vec3& target)
{
	_target = glm::normalize(target);
}

void SpringRotatedVector::setBackupAxis(const glm::vec3& backupAxis)
{
	_backupAxis = glm::normalize(backupAxis);
}

void SpringRotatedVector::setSpringConstant(float springConstant)
{
	_springConstant = springConstant;
}

void SpringRotatedVector::setDampingConstant(float dampingConstant)
{
	_dampingConstant = dampingConstant;
}

void SpringRotatedVector::setVector(const glm::vec3& vector)
{
	_vector = vector;
}
