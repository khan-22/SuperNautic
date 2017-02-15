#include "Core/Utility/SpringTranslatedVector.hpp"

SpringTranslatedVector::SpringTranslatedVector(const glm::vec3& vector, const glm::vec3& target, float dampingConstant)
	: _vector{ vector }, _target{ target }, _springConstant{ dampingConstant }, _velocity{ 0, 0, 0 }
{

}
void SpringTranslatedVector::update(float dt)
{
	glm::vec3 toTarget = _target - _vector;

	// http://mathproofs.blogspot.se/2013/07/critically-damped-spring-smoothing.html
	_velocity = (_velocity - _springConstant * _springConstant * dt * (-toTarget)) / powf(1 + _springConstant * dt, 2.0f);

	_vector = _vector + _velocity * dt;
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