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
		void setScale(const glm::vec3& scale);
		void setOrigin(const glm::vec3& origin);

		const glm::vec3& getPosition(const glm::vec3& position) const;
		const glm::vec3& getScale(const glm::vec3& scale) const;
		const glm::vec3& getOrigin(const glm::vec3& origin) const;

		void move(const glm::vec3& offset);
		void rotate(float angle, const glm::vec3& axis);
		void scale(const glm::vec3& factor);

		const glm::mat4& getTransformMatrix();

	protected:

	private:
		glm::vec3	_origin;
		glm::vec3	_position;
		float		_rotationAngle;
		glm::vec3	_rotationAxis;
		glm::vec3	_scale;

		glm::mat4	_transform;
		bool		_bTransformNeedsUpdate;

		glm::mat4	_inverseTransform;
		bool		_bInverseTransformNeedsUpdate;
	};

}

#endif //TRANSFORMABLE_3D_HPP
