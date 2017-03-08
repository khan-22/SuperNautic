#include "Core/Utility/SpringTranslatedVector.hpp"

SpringTranslatedVector::SpringTranslatedVector(const glm::vec3& vector, const glm::vec3& target, const glm::vec3& alternateAxis, float springConstant, float alternateSpringConstant)
	: _vector{ vector }, _target{ target }, _springConstant{ springConstant }, _velocity{ 0 }, _alternateAxis{ alternateAxis }, _alternateSpringConstant{ alternateSpringConstant }, _alternateVelocity{ 0 }
{

}
void SpringTranslatedVector::update(float dt)
{
	glm::vec3 toTarget = _target - _vector;

	// http://mathproofs.blogspot.se/2013/07/critically-damped-spring-smoothing.html
	_velocity = (_velocity - _springConstant * _springConstant * dt * (-toTarget)) / powf(1 + _springConstant * dt, 2.0f);
	_alternateVelocity = (_alternateVelocity - _alternateSpringConstant * _alternateSpringConstant * dt * (-toTarget)) / powf(1 + _alternateSpringConstant * dt, 2.0f);

	glm::vec3 movementAlongSlowAxis{ glm::dot(_alternateVelocity, _alternateAxis) * _alternateAxis };
	glm::vec3 otherMovement{ _velocity - glm::dot(_velocity, _alternateAxis) * _alternateAxis };

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

void SpringTranslatedVector::setAlternateSpringConstant(float alternateSpringConstant)
{
	_alternateSpringConstant = alternateSpringConstant;
}

void SpringTranslatedVector::setVector(const glm::vec3& vector)
{
	_vector = vector;
}

void SpringTranslatedVector::setAlternateAxis(const glm::vec3& slowAxis)
{
	_alternateAxis = slowAxis;
}
