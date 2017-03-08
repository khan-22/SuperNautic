#pragma once
#ifndef SEGMENTINSTANCE_HPP
#define SEGMENTINSTANCE_HPP

#include <glm/glm.hpp>

#include "Core/Geometry/Sphere.hpp"
#include "Core/Geometry/Ray.hpp"
#include "Core/Geometry/RayIntersection.hpp"
#include "Core/Track/Segment.hpp"
#include "Core/Asset/LoadAssetFunctions.hpp"
#include "GFX/Rendering/Renderable3D.hpp"
#include "Core/Track/ObstacleInstance.hpp"

class SegmentInstance : public GFX::Renderable3D
{
public:
	SegmentInstance(const Segment * segment, glm::mat4 modelMatrix, const bool lighting);
	virtual ~SegmentInstance();
	glm::mat4 getEndMatrix() const;
	glm::mat4 getModelMatrix() const;
	const float getLength() const;
    const std::vector<BoundingBox>& getGlobalBoundingBoxes() const;
	const int getIndex() const;
	void addObstacle(ObstacleInstance newObstacle);

	void update(const float dt);
	void decreaseSpeed(const float dt);
	void render(GFX::RenderStates& states) override;
	const RayIntersection rayIntersectionTest(const Ray& ray) const;
	std::vector<ObstacleInstance>& getObstacles();
	// Remove obstacle at index i
	void removeObstacle(size_t i);

	// Set temperature vector
	void setTemperatures(std::vector<float>& temperatures);

	const Segment* getParent()
	{
		return _parent;
	}

	bool bTestCollision(const SegmentInstance& other) const;

private:
    const Segment *					_parent;
	glm::mat4						_model;
	std::vector<BoundingBox>		_globalBoundingBoxes;
	std::vector<Sphere>				_globalBoundingSpheres;
	bool							_bHasLighting;
	std::vector<ObstacleInstance>	_obstacles;

	// temperatures[i] contains temperature of zone i
	std::vector<float>				_temperatures;

	SegmentInstance();
	void updateGlobalBounds();
	bool bTestCollisionSphere(const SegmentInstance& other) const;
	bool bTestCollisionBox(const SegmentInstance& other) const;
};

#endif // !SEGMENTINSTANCE_HPP
