#include <glm/gtx/transform.hpp>

#include "Core/Track/ObstacleInstance.hpp"


ObstacleInstance::ObstacleInstance(const glm::mat4& m, ObstacleHandler::Obstacle* parent, float diff)
	: _modelMat(m)
	, _parent(parent)
{
	_rotSpeed = _parent->getRandomRotSpeed(diff);
	_rotDir = 1;
	if (rand() % 2 == 0)
	{
		_rotDir = -1;
	}
}

ObstacleInstance::~ObstacleInstance()
{
	//Nothing
}

void ObstacleInstance::update(const float dt)
{
	_modelMat = _modelMat * glm::rotate(glm::radians(_rotSpeed * _rotDir * dt), glm::vec3(0, 0, 1));
}

void ObstacleInstance::decreaseSpeed(const float dt)
{
	if (_rotSpeed > 0.f)
	{
		_rotSpeed -= 5.0f * dt;
	}
}

void ObstacleInstance::render(GFX::RenderStates & states)
{
	_parent->getModel().getModelAsset().get()->setModelMatrix(_modelMat);
	_parent->getModel().render(states);
}

std::vector<BoundingBox>& ObstacleInstance::getBoundingBoxes() const
{
	return _parent->_boxes;
}

const glm::mat4 & ObstacleInstance::getModelMatrix() const
{
	return _modelMat;
}
