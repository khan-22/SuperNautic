#pragma once

#ifndef SPRINGROTATEDVECTOR_HPP
#define SPRINGROTATEDVECTOR_HPP

#include "glm/glm.hpp"

class SpringRotatedVector
{
public:
	SpringRotatedVector(const glm::vec3& vector, const glm::vec3& target, const glm::vec3& backupAxis, float springConstant, float dampingConstant);
	void update(float dt);

	const glm::vec3& operator()() const;

	const glm::vec3& getVector() const;

	void setTarget(const glm::vec3& target);

	void setBackupAxis(const glm::vec3& backupAxis);

	void setSpringConstant(float springConstant);

	void setDampingConstant(float dampingConstant);

	void setVector(glm::vec3& vector);

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
	// Force of damping, linear to velocity
	float _dampingConstant;
};

#endif
