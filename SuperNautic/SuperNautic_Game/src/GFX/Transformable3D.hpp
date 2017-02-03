#pragma once

#ifndef TRANSFORMABLE_3D_HPP
#define TRANSFORMABLE_3D_HPP

#include <glm/glm.hpp>

namespace GFX
{

	class Transformable3D
	{
	public:
		Transformable3D();
		virtual ~Transformable3D();

		void setPosition(const glm::vec3& position);
		void setRotation(float angle, const glm::vec3& axis);
		void setRotation(float x, float y, float z);
		void setScale(const glm::vec3& scale);
		void setScale(const float scale);
		void setOrigin(const glm::vec3& origin);

		// Creates lookAt matrix from forward and up directions (makes forward orthogonal to up)
		void setLookAt(const glm::vec3& forwardDirection, const glm::vec3& upDirection);

		const glm::vec3& getPosition() const;
		// _rotationMatrix only
		const glm::mat4& getRotation() const;
		const glm::mat4& getLookAt() const;
		const glm::vec3& getScale() const;
		const glm::vec3& getOrigin() const;

		void move(const glm::vec3& offset);
		void rotate(float angle, const glm::vec3& axis);
		void rotate(float x, float y, float z);
		void scale(const glm::vec3& factor);
		void scale(const float factor);

		const glm::mat4& getTransformMatrix();
		const glm::mat4& getInverseTransformMatrix();

	protected:

	private:
		// Local origin
		glm::vec3	_origin;
		glm::vec3	_position;
		glm::vec3	_scale;

		// Rotation around world axes
		glm::mat4	_rotationMatrix;

		glm::mat4	_transform;
		bool		_bTransformNeedsUpdate;

		// Defines up and forward for this object
		glm::mat4	_lookAt;

		glm::mat4	_inverseTransform;
		bool		_bInverseTransformNeedsUpdate;
	};

}

#endif //TRANSFORMABLE_3D_HPP
