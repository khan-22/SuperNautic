#include <fstream>

#include "Core/Track/ObstacleHandler.hpp"
#include "Core/Io/Log.hpp"
#include "Core/Utility/Utilities.hpp"
#include "Core/Asset/AssetCache.hpp"

std::string ObstacleHandler::_basePathObstacles = "res/models/obstacles/";

ObstacleHandler::ObstacleHandler(const std::string& infoFileName)
{
	std::ifstream in(_basePathObstacles + infoFileName);
	if (!in.is_open())
	{
		// File could not be opened
		LOG_ERROR("Could not open SegmentInfo file ", _basePathObstacles + infoFileName);
		return;
	}

	size_t amount;
	in >> amount;
	for (size_t i = 0; i < amount; i++)
	{
		std::string visName, dataName, matName;
		in >> visName >> dataName >> matName;

		GFX::TexturedModel model(ModelCache::get("obstacles/" + visName), MaterialCache::get(matName));

		RawMeshAsset asset = RawMeshCache::get("obstacles/" + dataName);
		std::vector<BoundingBox> boxes;
		boxes.reserve(asset.get()->meshes.size());
		for (size_t n = 0; n < asset.get()->meshes.size(); n++)
		{
			boxes.push_back(BoundingBox(asset.get()->meshes[n]));
		}

		int prob1, maxInRow1, padding1;
		float minRotSpeed1, maxRotSpeed1;
		in >> prob1 >> minRotSpeed1 >> maxRotSpeed1 >> maxInRow1 >> padding1;

		int prob2, maxInRow2, padding2;
		float minRotSpeed2, maxRotSpeed2;
		in >> prob2 >> minRotSpeed2 >> maxRotSpeed2 >> maxInRow2 >> padding2;

		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		_obstacles.push_back(new Obstacle(model, boxes
			, prob1, minRotSpeed1, maxRotSpeed1, maxInRow1, padding1
			, prob2, minRotSpeed2, maxRotSpeed2, maxInRow2, padding2));
	}
	in.close();
}

ObstacleHandler::~ObstacleHandler()
{
	for (size_t i = 0; i < _obstacles.size(); i++)
	{
		delete _obstacles[i];
	}
}

ObstacleHandler::Obstacle * ObstacleHandler::getRandomObstacle(float diff)
{
	int totalProb = 0;
	for (size_t i = 0; i < _obstacles.size(); i++)
	{
		totalProb += _obstacles[i]->getProbability(diff);
	}
	int r = rand() % totalProb;
	int tried = 0;
	size_t index = 0;
	while (tried <= r)
	{
		tried += _obstacles[index]->getProbability(diff);
		index++;
	}
	index--;
	return _obstacles[index];
}
