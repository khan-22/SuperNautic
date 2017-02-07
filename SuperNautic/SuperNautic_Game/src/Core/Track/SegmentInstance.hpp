#pragma once
#ifndef SEGMENTINSTANCE_HPP
#define SEGMENTINSTANCE_HPP

#include <glm\glm.hpp>

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
	void render(GFX::RenderStates& states) override;

private:
	SegmentInstance();

	const Segment *		_parent;
	glm::mat4			_model;
	bool				_bHasLighting;
};

#endif // !SEGMENTINSTANCE_HPP
