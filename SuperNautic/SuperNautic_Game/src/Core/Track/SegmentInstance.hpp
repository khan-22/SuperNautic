#pragma once
#ifndef SEGMENTINSTANCE_HPP
#define SEGMENTINSTANCE_HPP

#include <glm\glm.hpp>

#include "Segment.hpp"

class SegmentInstance
{
public:
	SegmentInstance(const Segment * segment, const glm::mat4& modelMatrix, const bool lighting);
	virtual ~SegmentInstance();
	glm::mat4 getEndMatrix() const;
	glm::mat4 getModelMatrix() const;

private:
	SegmentInstance();

	const Segment *		_parent;
	glm::mat4			_model;
	bool				_bHasLighting;
};

#endif // !SEGMENTINSTANCE_HPP
