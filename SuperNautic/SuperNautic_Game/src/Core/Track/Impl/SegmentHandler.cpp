#include <fstream>
#include <limits>

#include "../SegmentHandler.hpp"
#include "../../../Log.hpp"

// Path to res\models
std::string SegmentHandler::basePath = "res/models/";

// Loads SegmentInfos and connections from file (relative to res/models)
SegmentHandler::SegmentHandler(std::string segmentInfoPath, std::string connectionInfoPath)
{
	// Read SegmentInfos
	std::ifstream infoFile { basePath + segmentInfoPath };
	if (!infoFile.is_open())
	{
		// File could not be opened
		LOG_ERROR("Could not open SegmentInfo file ", basePath + segmentInfoPath);
		return;
	}

	std::string segmentDataName;
	std::string segmentVisualName;
	char startConnection;
	char endConnection;
	int probability;
	int minInRow;
	int maxInRow;
	int rotationOffset;

	while (infoFile >> segmentDataName) // Read data file name
	{
		infoFile >> segmentVisualName;
		// blah_aa.blend
		//      ^^
		// size - 8, size - 7 contains connection info
		startConnection = segmentDataName[segmentDataName.size() - 8];
		endConnection = segmentDataName[segmentDataName.size() - 7];

		infoFile >> probability;
		infoFile >> minInRow;
		infoFile >> maxInRow;
		infoFile >> rotationOffset;

		// Add read SegmentInfo to vector
		_segmentInfos.push_back(SegmentInfo{ std::move(segmentDataName), std::move(segmentVisualName)
			, startConnection, endConnection, probability, minInRow, maxInRow, rotationOffset });

		// Skip rest of line
		infoFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	infoFile.close();


	// Read connections
	infoFile.open(basePath + connectionInfoPath);
	if (!infoFile.is_open())
	{
		// File could not be opened
		LOG_ERROR("Could not open SegmentInfo file ", basePath + connectionInfoPath);
		return;
	}

	char type;
	int amount, rotation;
	infoFile >> amount;
	for (unsigned int i = 0; i < amount; i++)
	{
		infoFile >> type;
		infoFile >> rotation;
		std::pair<char, int> temp(type, rotation);
		_connections.insert(temp);
	}
	infoFile.close();
}

// Destructor
SegmentHandler::~SegmentHandler()
{
	for (unsigned int i = 0; i < _segments.size(); i++)
	{
		delete _segments[i];
	}
}

// Load segment from fbx file using info from _segments[i], returns reference to loaded segment
// Just returns reference if already loaded
const Segment* SegmentHandler::loadSegment(unsigned i)
{
	// If already loaded, just return reference
	if (_segmentInfos[i].loadedIndex != -1)
	{
		return _segments[_segmentInfos[i].loadedIndex];
	}
	// Else load Segment, then return
	else
	{
		// Load Segment from file
		_segments.push_back(new Segment(&_segmentInfos[i]));

		// Set index in corresponding SegmentInfo
		_segmentInfos[i].loadedIndex = static_cast<int>(_segments.size()) - 1;

		return _segments[_segmentInfos[i].loadedIndex];
	}
}

// Returns rotation info about connection type 'type'
int SegmentHandler::getConnectionRotation(const char type)
{
	return _connections[type];
}
