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
		int							_maxInRow1;
		int							_padding1;
		int							_probability2;
		float						_minRotSpeed2;
		float						_maxRotSpeed2;
		int							_maxInRow2;
		int							_padding2;

		Obstacle(const GFX::TexturedModel& model
			, const std::vector<BoundingBox>& boxes
			, int prob1, float minRotSpeed1, float maxRotSpeed1, int maxInRow1, int padding1
			, int prob2, float minRotSpeed2, float maxRotSpeed2, int maxInRow2, int padding2)
			: _model(model)
			, _boxes(boxes)
			, _probability1(prob1)
			, _minRotSpeed1(minRotSpeed1)
			, _maxRotSpeed1(maxRotSpeed1)
			, _maxInRow1(maxInRow1)
			, _padding1(padding1)
			, _probability2(prob2)
			, _minRotSpeed2(minRotSpeed2)
			, _maxRotSpeed2(maxRotSpeed2)
			, _maxInRow2(maxInRow1)
			, _padding2(padding2)
		{ }
		GFX::TexturedModel getModel() const
		{
			return _model;
		}
		int getProbability(float diff) const
		{
			return _probability1 * (1 - diff) + _probability2 * diff;
		}
		float getMinRotSpeed(float diff) const
		{
			return _minRotSpeed1 * (1 - diff) + _minRotSpeed2 * diff;
		}
		float getMaxRotSpeed(float diff) const
		{
			return _maxRotSpeed1 * (1 - diff) + _maxRotSpeed2 * diff;
		}
		float getRandomRotSpeed(float diff) const
		{
			float min = getMinRotSpeed(diff);
			float max = getMaxRotSpeed(diff);
			double scaled = (double)rand() / RAND_MAX;
			float rotDir = 1;
			if (rand() % 2 == 0)
			{
				rotDir = -1;
			}
			return ((max - min + 1) * scaled + min) * rotDir;
		}
		int	getMaxInRow(float diff)
		{
			return _maxInRow1 * (1 - diff) + _maxInRow2 * diff;
		}
		float getPadding(float diff)
		{
			return _padding1 * (1 - diff) + _padding2 * diff;
		}
	};

	ObstacleHandler(const std::string& infoFileName);
	virtual ~ObstacleHandler();
	Obstacle* getRandomObstacle(float diff);

private:
	std::vector<Obstacle*>		_obstacles;
	static std::string			_basePathObstacles;
};

#endif // !OBSTACLEHANDLER_H

