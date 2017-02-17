#pragma once

#ifndef DAMPEDSPRINGROTATEDVECTOR_HPP
#define DAMPEDSPRINGROTATEDVECTOR_HPP

#include "glm/glm.hpp"
#include "Core/Utility/SpringRotatedVector.hpp"

class DampedSpringRotatedVector : public SpringRotatedVector
{
public:
	DampedSpringRotatedVector(const glm::vec3& vector, const glm::vec3& target, const glm::vec3& backupAxis, float springConstant);
	void update(float dt) override;

	virtual ~DampedSpringRotatedVector() {};


private:

};

#endif
