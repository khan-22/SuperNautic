#include "SegmentInstance.hpp"

//Default constructor (private)
SegmentInstance::SegmentInstance()
{
	//Not used
}

//Real constructor
SegmentInstance::SegmentInstance(const Segment * segment, glm::mat4 modelMatrix, const bool lighting)
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

void SegmentInstance::render(GFX::RenderStates & states)
{
	_parent->getVisualModel().get()->setModelMatrix(_model);
	_parent->getVisualModel().get()->render(states);
}

const RayIntersection SegmentInstance::rayIntersectionTest(const Ray& ray) const
{
	glm::mat4 inverse = glm::inverse(_model);

	Ray localRay{ inverse * glm::vec4{ ray.origin().x , ray.origin().y, ray.origin().z, 1.0f },
				  inverse * glm::vec4{ ray.direction().x, ray.direction().y, ray.direction().z, 0.0f }, 
				  ray.length() };

	return _parent->rayIntersectionTest(localRay);
}
