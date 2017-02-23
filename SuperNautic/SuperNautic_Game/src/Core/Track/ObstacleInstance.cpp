#include <glm/gtx/transform.hpp>
//#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Core/Track/ObstacleInstance.hpp"


ObstacleInstance::ObstacleInstance(const glm::vec3& pos, const glm::vec3& forward, glm::mat4 m
	, ObstacleHandler::Obstacle* parent, float diff)
	: _pos(pos)
	, _forwardDir(forward)
	, _parent(parent)
{
	//glm::mat4 t = glm::translate(pos);
	//float a = glm::dot(glm::normalize(forward), glm::vec3(0, 1, 0));
	//float a = acosf(glm::length(forward) / glm::length(glm::vec3(0, 1, 0)));
	//glm::mat4 ownRot = glm::rotate(a, glm::vec3(1, 0, 0));
	//glm::mat4 rotAroundItself = glm::rotate(glm::radians(float(rand() % 360)), forward);
	//_modelMat = m * t * ownRot; // * rotAroundItself;
	_modelMat = m * glm::inverse(glm::lookAt(pos, pos + forward, glm::vec3(0, 1, 0)));

	_rotSpeed = _parent->getRandomRotSpeed(diff);
}

ObstacleInstance::~ObstacleInstance()
{
	//Nothing
}

void ObstacleInstance::update(const float dt)
{
	_modelMat = _modelMat * glm::rotate(glm::radians(_rotSpeed * dt), _forwardDir);
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
