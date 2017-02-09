#pragma once
#ifndef SEGMENTINSTANCE_HPP
#define SEGMENTINSTANCE_HPP

#include <glm\glm.hpp>

#include "../Geometric Primitives/Sphere.hpp"
#include "Segment.hpp"
#include "../LoadAssetFunctions.hpp"
#include "../../GFX/Renderable3D.hpp"

class SegmentInstance : public GFX::Renderable3D
{
public:
	SegmentInstance(const Segment * segment, glm::mat4 modelMatrix, const bool lighting);
	virtual ~SegmentInstance();
	glm::mat4 getEndMatrix() const;
	glm::mat4 getModelMatrix() const;
	const int getLength() const;
    const std::vector<BoundingBox>& getGlobalBoundingBoxes() const;

	void render(GFX::RenderStates& states) override;

	bool bTestCollision(const SegmentInstance& other) const;

private:
    const Segment *		_parent;
	glm::mat4			_model;
	std::vector<BoundingBox> _globalBoundingBoxes;
	std::vector<Sphere> _globalBoundingSpheres;
	bool				_bHasLighting;

	SegmentInstance();
	void updateGlobalBounds();
	bool bTestCollisionSphere(const SegmentInstance& other) const;
	bool bTestCollisionBox(const SegmentInstance& other) const;


};

#endif // !SEGMENTINSTANCE_HPP
