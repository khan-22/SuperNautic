#include <algorithm>

#include "Core/Track/SegmentInstance.hpp"
#include "Core/Utility/CollisionUtility.hpp"
#include "Core/Utility/GeometryUtility.hpp"

//Default constructor (private)
SegmentInstance::SegmentInstance()
{
	//Not used
}

//Real constructor
SegmentInstance::SegmentInstance(const Segment * segment, glm::mat4 modelMatrix)
{
	_parent = segment;
	_model = modelMatrix;
	updateGlobalBounds();

	for (unsigned int i = 0; i < _parent->nrOfZones(); ++i)
	{
		_temperatures.push_back(-5.0f);
	}
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

const float SegmentInstance::getLength() const
{
	return _parent->getLength();
}

const int SegmentInstance::getIndex() const
{
	return _parent->getInfo()->loadedIndex;
}

void SegmentInstance::addObstacle(ObstacleInstance newObstacle)
{
	_obstacles.push_back(newObstacle);
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
            globalBox.directions[i] = glm::normalize(_model * glm::vec4(localBox.directions[i], 0.f));
        }
        globalBox.halfLengths = localBox.halfLengths;
    }

    _globalBoundingSpheres.clear();
    for(const BoundingBox& globalBox : _globalBoundingBoxes)
    {
        _globalBoundingSpheres.emplace_back(generateBoundingSphere(globalBox));
    }
}


void SegmentInstance::update(const float dt)
{
	for (size_t i = 0; i < _obstacles.size(); ++i)
	{
		_obstacles[i].update(dt);
	}
}

void SegmentInstance::decreaseSpeed(const float dt)
{
	for (unsigned int i = 0; i < _obstacles.size(); ++i)
	{
		_obstacles[i].decreaseSpeed(dt);
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

	RayIntersection intersection = _parent->rayIntersectionTest(localRay, _temperatures);

	// Transform to global coordinates if a hit is detected
	if (intersection)
	{
		intersection._position = glm::vec3{ _model * glm::vec4{ intersection._position.x , intersection._position.y, intersection._position.z, 1.0f } };
		intersection._normal = glm::vec3{ _model * glm::vec4{ intersection._normal.x , intersection._normal.y, intersection._normal.z, 0.0f } };
	}

	return intersection;
}

std::vector<ObstacleInstance>& SegmentInstance::getObstacles()
{
	return _obstacles;
}

void SegmentInstance::removeObstacle(size_t i)
{
	assert(i < _obstacles.size());

	_obstacles.erase(_obstacles.begin() + i);
}

void SegmentInstance::setTemperatures(std::vector<float>& temperatures)
{
	_temperatures = temperatures;
}
