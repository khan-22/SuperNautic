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

std::vector<BoundingBox> SegmentInstance::getGlobalBounds() const
{
    std::vector<BoundingBox> boxes;
    for(const BoundingBox& localBox : _parent->getBoundingBoxes())
    {
        boxes.emplace_back();
        BoundingBox& globalBox = boxes.back();


        globalBox.center = glm::vec4(localBox.center, 1.f) * _model;
        for(size_t i = 0; i < localBox.directions.size(); i++)
        {
            globalBox.directions[i] = glm::vec4(localBox.directions[i], 0.f) * _model;
        }
        globalBox.halfLengths = localBox.halfLengths;
    }

    return boxes;
}


void SegmentInstance::render(GFX::RenderStates & states)
{
	_parent->getVisualModel().get()->setModelMatrix(_model);
	_parent->getVisualModel().get()->render(states);
}
