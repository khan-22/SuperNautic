#include "SegmentInstance.hpp"

//Default constructor (private)
SegmentInstance::SegmentInstance()
{
	//Not used
}

//Real constructor
SegmentInstance::SegmentInstance(const Segment * segment, const glm::mat4& modelMatrix, const bool lighting)
{
	_parent = segment;
	_model = modelMatrix;
	_bHasLighting = lighting;
}

//Destructor
SegmentInstance::~SegmentInstance()
{
	//Nothing
}

glm::mat4 SegmentInstance::getEndMatrix() const
{
	return _parent->getEndMatrix();
}

glm::mat4 SegmentInstance::getModelMatrix() const
{
	return _model;
}
