#include "SpringSmoothedValue.hpp"
#include <math.h>

SpringSmoothedValue::SpringSmoothedValue(float value, float target, float springConstant)
	: _value{ value }, _target{ target }, _springConstant{ springConstant }, _velocity{ 0.0f }
{

}

void SpringSmoothedValue::update(float dt)
{
	float toTarget = _target - _value;

	// http://mathproofs.blogspot.se/2013/07/critically-damped-spring-smoothing.html
	_velocity = (_velocity - _springConstant * _springConstant * dt * (-toTarget)) / powf(1 + _springConstant * dt, 2.0f);

	_value = _value + _velocity * dt;
}

float SpringSmoothedValue::operator()() const
{
	return _value;
}

float SpringSmoothedValue::getValue() const
{
	return (*this)();
}

void SpringSmoothedValue::setTarget(float target)
{
	_target = target;
}

void SpringSmoothedValue::setSpringConstant(float springConstant)
{
	_springConstant = springConstant;
}

void SpringSmoothedValue::setValue(float value)
{
	_value = value;
}
