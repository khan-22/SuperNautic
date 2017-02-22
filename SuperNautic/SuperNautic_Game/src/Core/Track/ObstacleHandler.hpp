#pragma once
#ifndef OBSTACLEHANDLER_HPP
#define OBSTACLEHANDLER_HPP

#include <string>

#include "GFX/Resources/TexturedModel.hpp"
#include "Core/Geometry/BoundingBox.hpp"

class ObstacleHandler
{
public:
	struct Obstacle
	{
		GFX::TexturedModel			_model;
		std::vector<BoundingBox>	_boxes;
		int							_probability1;
		float						_minRotSpeed1;
		float						_maxRotSpeed1;
		int							_padding1;

		Obstacle(const GFX::TexturedModel& model
			, const std::vector<BoundingBox>& boxes
			, int prob1, float minRotSpeed1, float maxRotSpeed1
			, int padding1)
			: _model(model)
			, _boxes(boxes)
			, _probability1(prob1)
			, _minRotSpeed1(minRotSpeed1)
			, _maxRotSpeed1(maxRotSpeed1)
			, _padding1(padding1)
		{ }
		int getProbability(float diff) const
		{
			return _probability1;
		}
		float getMinRotSpeed(float diff) const
		{
			return _minRotSpeed1;
		}
		float getMaxRotSpeed(float diff) const
		{
			return _maxRotSpeed1;
		}
		float getRandomRotSpeed(float diff) const
		{
			int min = getMinRotSpeed(diff);
			int max = getMaxRotSpeed(diff);
			double scaled = (double)rand() / RAND_MAX;
			float rotDir = 1;
			if (rand() % 2 == 0)
			{
				rotDir = -1;
			}
			return ((max - min + 1) * scaled + min) * rotDir;
		}
		GFX::TexturedModel getModel() const
		{
			return _model;
		}
		int getPadding(float diff) const
		{
			return _padding1;
		}
	};

	ObstacleHandler(const std::string& infoFileName);
	virtual ~ObstacleHandler();
	Obstacle* getRandomObstacle();

private:
	std::vector<Obstacle*>		_obstacles;
	static std::string			_basePathObstacles;
};

#endif // !OBSTACLEHANDLER_H

