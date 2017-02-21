#include <fstream>

#include "Core/Track/ObstacleHandler.hpp"
#include "Core/Io/Log.hpp"

std::string ObstacleHandler::_basePath = "res/models/obstacles/";

ObstacleHandler::ObstacleHandler(const std::string& filename)
{
	std::ifstream in(_basePath + filename);
	if (!in.is_open())
	{
		// File could not be opened
		LOG_ERROR("Could not open SegmentInfo file ", _basePath + filename);
		return;
	}

	size_t amount;
	in >> amount;
	for (size_t i = 0; i < amount; i++)
	{
		std::string visName, dataName;
		in >> visName >> dataName;

		int prob1, minRotSpeed1, maxRotSpeed1;
		in >> prob1 >> minRotSpeed1 >> maxRotSpeed1;

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