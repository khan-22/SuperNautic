#pragma once

#ifndef AABB_H
#define AABB_H

#include <vector>

#include "glm/vec3.hpp"
#include "Core/Geometry/Ray.hpp"
#include "Core/Geometry/RayIntersection.hpp"

// Axis aligned bounding box used for oct-tree
struct AABB
{
	// The children of this box
	std::vector<AABB>					_children;

	// Will contain indices into face vectors of models
	// [0]..[_temperatureZoneCollisions.size()] is zone collisions, [_temperatureZoneCollisions.size()] is base collision
	std::vector<std::vector<unsigned>> faceIndices;

	// Positions of max and min corners of this box
	glm::vec3							_minCorner;
	glm::vec3							_maxCorner;

	// Test if a ray intersects this box, returns intersection position along ray, -1.0 if no hit
	float rayIntersection(const Ray& ray) const
	{
		float tmin = 0;
		float tmax = ray.length();

		for (unsigned i = 0; i < 3; ++i)
		{
			float t0 = (_minCorner[i] - ray.origin()[i]) * ray.invDirection()[i];
			float t1 = (_maxCorner[i] - ray.origin()[i]) * ray.invDirection()[i];

			if (ray.invDirection()[i] < 0.0f)
			{
				std::swap(t0, t1);
			}

			tmin = t0 > tmin ? t0 : tmin;
			tmax = t1 < tmax ? t1 : tmax;

			if (tmax <= tmin)
				return -1.0f;
		}
		return (tmin <= ray.length()) ? tmin : -1.0f;
	}

	// Test if a ray intersects any geometry in this box
	RayIntersection triangleRayIntersection(const Ray& ray, const std::vector<unsigned>& modelIndices, const std::vector<SurfaceType>& temperatures, const RawMeshAsset& scene) const
	{
		// Will hold final intersection
		RayIntersection intersection{ false };

		// For every model in segment
		for (unsigned i = 0; i < modelIndices.size(); ++i)
		{
			const unsigned currentModel = modelIndices[i];

			std::vector<glm::uvec3>& faces{ scene.get()->meshes[currentModel].faces };
			std::vector<glm::vec3>& vertices{ scene.get()->meshes[currentModel].vertices };

			// For every triangle in model
			for (unsigned j = 0; j < scene.get()->meshes[currentModel].faces.size(); ++j)
			{
				// Vertices of triangle
				glm::vec3& v0 = vertices[faces[j].x];
				glm::vec3& v1 = vertices[faces[j].y];
				glm::vec3& v2 = vertices[faces[j].z];

				// Two triangle edges
				glm::vec3 e0 = v1 - v0;
				glm::vec3 e1 = v2 - v0;

				glm::vec3 pvec = glm::cross(ray.direction(), e1);

				float det = glm::dot(e0, pvec);

				// Ray and triangle is in the same plane
				if (det < 0.00001f)
				{
					continue;
				}

				float inv_det = 1.0f / det;
				glm::vec3 tvec = ray.origin() - v0;

				float u = glm::dot(tvec, pvec) * inv_det;

				// Intersection is outside triangle
				if (u < 0.0f || u > 1.0f)
				{
					continue;
				}

				glm::vec3 qvec = glm::cross(tvec, e0);

				float v = glm::dot(ray.direction(), qvec) * inv_det;

				// Intersection is outside triangle
				if (v < 0.0f || u + v > 1.0f)
				{
					continue;
				}

				float t = glm::dot(e1, qvec) * inv_det;

				// If no previous hit or this hit is closer and within range, record hit
				if ((!intersection || t < intersection._length) && t < ray.length() && t >= 0.0f)
				{
					intersection._length = t;
					intersection._hit = true;

					// Return surface type using temperatures vector
					if (i < modelIndices.size() - 1) // The last model is the base, others are temperature zones
					{
						if (temperatures.size() != modelIndices.size() - 1)
						{
							LOG_ERROR("Temperatures vector size does not correspond to number of temperature zone models!");
							abort();
						}

						intersection._surface = temperatures[i];
					}
					else
					{
						intersection._surface = SurfaceType::normal;
					}

					intersection._position = v0 * (1.0f - u - v) + v1 * u + v2 * v;
					intersection._normal = scene.get()->meshes[currentModel].normals[faces[j].x] * (1.0f - u - v) +
										   scene.get()->meshes[currentModel].normals[faces[j].y] * u +
										   scene.get()->meshes[currentModel].normals[faces[j].z] * v;
				}
			}
		}

		return intersection;
	}

	AABB(glm::vec3 min, glm::vec3 max)
		: _minCorner{ min }, _maxCorner{ max }
	{ }

	AABB()
	{ }
};

#endif
