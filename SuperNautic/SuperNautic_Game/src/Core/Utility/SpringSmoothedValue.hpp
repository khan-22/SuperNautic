#pragma once

#ifndef SPRINGSMOOTHEDVALUE_H
#define SPRINGSMOOTHEDVALUE_H

class SpringSmoothedValue
{
public:
	SpringSmoothedValue(float value, float target, float springConstant);
	void update(float dt);

	float operator()() const;

	float getValue() const;

	void setTarget(float target);

	void setSpringConstant(float springConstant);

	void setValue(float value);

private:
	// The smoothed value
	float _value;
	// The smoothing target
	float _target;
	// The current velocity
	float _velocity;

	// Force of spring
	float _springConstant;
};

#endif
