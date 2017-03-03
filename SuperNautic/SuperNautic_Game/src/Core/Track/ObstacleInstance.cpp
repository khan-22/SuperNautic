#include <glm/gtx/transform.hpp>

#include "Core/Track/ObstacleInstance.hpp"


ObstacleInstance::ObstacleInstance(const glm::mat4& m, ObstacleHandler::Obstacle* parent, float diff)
	: _modelMat(m)
	, _parent(parent)
{
	_rotSpeed = _parent->getRandomRotSpeed(diff);
}

ObstacleInstance::~ObstacleInstance()
{
	//Nothing
}

void ObstacleInstance::update(const float dt)
{
	_modelMat = _modelMat * glm::rotate(glm::radians(_rotSpeed * dt), glm::vec3(0,0,1));
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
