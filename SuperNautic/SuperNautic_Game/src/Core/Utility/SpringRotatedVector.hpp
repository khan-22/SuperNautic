#pragma once

#ifndef SPRINGROTATEDVECTOR_H
#define SPRINGROTATEDVECTOR_H

#include "glm/glm.hpp"

class SpringRotatedVector
{
public:
	SpringRotatedVector(const glm::vec3& vector, const glm::vec3& target, const glm::vec3& backupAxis, float springConstant, float dampingConstant);
	void update(float dt);

	const glm::vec3& operator()() const
	{
		return _vector;
	}

	const glm::vec3& getVector() const
	{
		return (*this)();
	}

	void setTarget(const glm::vec3& target)
	{
		_target = glm::normalize(target);
	}

	void setBackupAxis(const glm::vec3& backupAxis)
	{
		_backupAxis = glm::normalize(backupAxis);
	}

	void setSpringConstant(float springConstant)
	{
		_springConstant = springConstant;
	}

	void setDampingConstant(float dampingConstant)
	{
		_dampingConstant = dampingConstant;
	}

	void setVector(glm::vec3& vector)
	{
		_vector = vector;
	}

private:
	// The rotating vector
	glm::vec3 _vector;
	// The rotation target
	glm::vec3 _target;
	// Axis of rotation, length is velocity
	glm::vec3 _axis;
	// Used for rotation if target and vector are opposites
	glm::vec3 _backupAxis;

	// Spring strength
	float _springConstant;
	// Force of damping linear to velocity
	float _dampingConstant;
};

#endif
