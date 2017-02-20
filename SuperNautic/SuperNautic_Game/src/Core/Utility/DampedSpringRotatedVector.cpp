#include "Core/Utility/DampedSpringRotatedVector.hpp"
#include "Core/Utility/Utilities.hpp"
#include "glm/gtx/vector_angle.hpp"

//DampedSpringRotatedVector::DampedSpringRotatedVector(const glm::vec3& vector, const glm::vec3& target, const glm::vec3& backupAxis, float springConstant)
//	: SpringRotatedVector{ vector, target, backupAxis, springConstant, 0 }
//{ }
//void DampedSpringRotatedVector::update(float dt)
//{
//	glm::vec3 newAxis = glm::normalize(glm::cross(_vector, _target));
//
//	// Find angle between vectors
//	float angleBetween;
//	float dot = glm::dot(_vector, _target);
//	if (dot >= 0.99f)
//	{
//		angleBetween = 0.0f;
//		newAxis = _backupAxis;
//	}
//	else if (dot <= -0.99f)
//	{
//		angleBetween = glm::pi<float>();
//		newAxis = _backupAxis;
//	}
//	else
//	{
//		angleBetween = acosf(dot);
//	}
//
//	if (!std::isnan(newAxis.x))
//	{
//		// http://mathproofs.blogspot.se/2013/07/critically-damped-spring-smoothing.html
//		_axis = glm::normalize(newAxis) * ((glm::length(_axis) - _springConstant * _springConstant * dt * angleBetween) / powf(1 + _springConstant * dt, 2.0f));
//	}
//
//	// Rotate vector
//	if (!bAlmostEqual(_axis, glm::vec3{ 0, 0, 0 }) && !isnan(_axis.x))
//	{
//		glm::mat4 rotation = glm::rotate(-glm::length(_axis) * dt, glm::normalize(_axis));
//
//		_vector = rotation * glm::vec4{ _vector, 0.0f };
//	}
//}
