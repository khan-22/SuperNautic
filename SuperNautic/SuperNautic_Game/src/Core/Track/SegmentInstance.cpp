#include <algorithm>

#include "SegmentInstance.hpp"
#include "../CollisionUtility.hpp"

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
	updateGlobalBounds();
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

const int SegmentInstance::getLength() const
{
	return _parent->getLength();
}

const int SegmentInstance::getIndex() const
{
	return _parent->_segmentInfo->loadedIndex;
}

bool SegmentInstance::bTestCollisionSphere(const SegmentInstance& other) const
{
    for(const Sphere& a : _globalBoundingSpheres)
    {
        for(const Sphere& b : other._globalBoundingSpheres)
        {
            if(::bTestCollision(a, b))
            {
                return true;
            }
        }
    }

    return false;
}

bool SegmentInstance::bTestCollisionBox(const SegmentInstance& other) const
{
    for(const BoundingBox& a : _globalBoundingBoxes)
    {
        for(const BoundingBox& b : other._globalBoundingBoxes)
        {
            if(::bTestCollision(a, b))
            {
                return true;
            }
        }
    }

    return false;
}

bool SegmentInstance::bTestCollision(const SegmentInstance& other) const
{
    if(!bTestCollisionSphere(other))
    {
        return false;
    }

    if(!bTestCollisionBox(other))
    {
        return false;
    }

    return true;
}

const std::vector<BoundingBox>& SegmentInstance::getGlobalBoundingBoxes() const
{
    return _globalBoundingBoxes;
}


void SegmentInstance::updateGlobalBounds()
{
    _globalBoundingBoxes.clear();
    for(const BoundingBox& localBox : _parent->getBoundingBoxes())
    {
        _globalBoundingBoxes.emplace_back();
        BoundingBox& globalBox = _globalBoundingBoxes.back();


        globalBox.center = _model * glm::vec4(localBox.center, 1.f);
        for(size_t i = 0; i < localBox.directions.size(); i++)
        {
            globalBox.directions[i] = _model * glm::vec4(localBox.directions[i], 0.f);
        }
        globalBox.halfLengths = localBox.halfLengths;
    }

    _globalBoundingSpheres.clear();
    for(const BoundingBox& globalBox : _globalBoundingBoxes)
    {
        //float maxHalfLength = *std::max_element(globalBox.halfLengths.begin(), globalBox.halfLengths.end());
        float radius = 0.f;
        for(const float& halfLength : globalBox.halfLengths)
        {
            radius += halfLength * halfLength;
        }
        radius = std::sqrt(radius);
        _globalBoundingSpheres.emplace_back(globalBox.center, radius);
    }
}


void SegmentInstance::render(GFX::RenderStates & states)
{
	_parent->getVisualModel().getModelAsset().get()->setModelMatrix(_model);
	_parent->getVisualModel().render(states);
}

const RayIntersection SegmentInstance::rayIntersectionTest(const Ray& ray) const
{
	glm::mat4 inverse = glm::inverse(_model);

	Ray localRay{ inverse * glm::vec4{ ray.origin().x , ray.origin().y, ray.origin().z, 1.0f },
				  inverse * glm::vec4{ ray.direction().x, ray.direction().y, ray.direction().z, 0.0f }, 
				  ray.length() };

	return _parent->rayIntersectionTest(localRay);
}
