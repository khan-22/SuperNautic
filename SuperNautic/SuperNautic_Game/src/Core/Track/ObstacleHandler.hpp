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

		Obstacle(const GFX::TexturedModel& model
			, const std::vector<BoundingBox>& boxes
			, int prob1, float minRotSpeed1, float maxRotSpeed1)
			: _model(model)
			, _boxes(boxes)
			, _probability1(prob1)
			, _minRotSpeed1(minRotSpeed1)
			, _maxRotSpeed1(maxRotSpeed1)
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
	};

	ObstacleHandler(const std::string& filename);
	virtual ~ObstacleHandler();

private:
	std::vector<Obstacle*>		_obstacles;
	static std::string			_basePath;
};

#endif // !OBSTACLEHANDLER_H

