#include "Core/Utility/SpringTranslatedVector.hpp"

SpringTranslatedVector::SpringTranslatedVector(const glm::vec3& vector, const glm::vec3& target, const glm::vec3& slowAxis, float springConstant, float slowSpringConstant)
	: _vector{ vector }, _target{ target }, _springConstant{ springConstant }, _velocity{ 0 }, _slowAxis{ slowAxis }, _slowSpringConstant{ slowSpringConstant }, _slowVelocity{ 0 }
{

}
void SpringTranslatedVector::update(float dt)
{
	glm::vec3 toTarget = _target - _vector;

	// http://mathproofs.blogspot.se/2013/07/critically-damped-spring-smoothing.html
	_velocity = (_velocity - _springConstant * _springConstant * dt * (-toTarget)) / powf(1 + _springConstant * dt, 2.0f);
	_slowVelocity = (_slowVelocity - _slowSpringConstant * _slowSpringConstant * dt * (-toTarget)) / powf(1 + _slowSpringConstant * dt, 2.0f);

	glm::vec3 movementAlongSlowAxis{ glm::dot(_slowVelocity, _slowAxis) * _slowAxis };
	glm::vec3 otherMovement{ _velocity - glm::dot(_velocity, _slowAxis) * _slowAxis };

	_vector = _vector + (otherMovement + movementAlongSlowAxis) * dt;
}

const glm::vec3& SpringTranslatedVector::operator()() const
{
	return _vector;
}

const glm::vec3& SpringTranslatedVector::getVector() const
{
	return (*this)();
}

void SpringTranslatedVector::setTarget(const glm::vec3& target)
{
	_target = target;
}

void SpringTranslatedVector::setSpringConstant(float springConstant)
{
	_springConstant = springConstant;
}

void SpringTranslatedVector::setVector(const glm::vec3& vector)
{
	_vector = vector;
}

void SpringTranslatedVector::setSlowAxis(const glm::vec3& slowAxis)
{
	_slowAxis = slowAxis;
}
