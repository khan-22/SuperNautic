#pragma once
#ifndef OBSTACLEINSTANCE_HPP
#define OBSTACLEINSTANCE_HPP

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "Core/Track/ObstacleHandler.hpp"
#include "GFX/Rendering/Renderable3D.hpp"

class ObstacleInstance : public GFX::Renderable3D
{
public:
	ObstacleInstance(const glm::mat4& m, ObstacleHandler::Obstacle* parent, float diff);
	virtual ~ObstacleInstance();
	void update(const float dt);
	void decreaseSpeed(const float dt);
	void render(GFX::RenderStates& states) override;
	std::vector<BoundingBox>& getBoundingBoxes() const;
	const glm::mat4& getModelMatrix() const;

private:
	glm::mat4					_modelMat;
	ObstacleHandler::Obstacle*	_parent;
	float						_rotSpeed;
	int							_rotDir;
	bool						_bDecreaseSpeed;
};

#endif // !OBSTACLEINSTANCE_HPP
