#include <fstream>
#include <limits>

#include "../SegmentHandler.hpp"
#include "../../../Log.hpp"

// Path to res\models
std::string SegmentHandler::basePath = "res\\models\\";

// Loads SegmentInfos from file with name filePath (relative to res/models)
SegmentHandler::SegmentHandler(std::string filePath)
{
	std::ifstream infoFile { basePath + filePath };

	if (!infoFile.is_open())
	{
		// File could not be opened
		LOG_ERROR("Could not open SegmentInfo file ", basePath + filePath);
		return;
	}

	std::string segmentDataName;
	std::string segmentVisualName;
	std::string startConnection;
	std::string endConnection;

	// Read SegmentInfos
	while (infoFile >> segmentDataName) // Read data file name
	{
		infoFile >> segmentVisualName;
		// blah_aa.fbx
		//      ^^
		// size - 6, size - 5 contains connection info
		startConnection = segmentDataName[segmentDataName.size() - 6];
		endConnection = segmentDataName[segmentDataName.size() - 5];

		// Add read SegmentInfo to vector
		_segmentInfos.push_back(SegmentInfo{ std::move(segmentDataName), std::move(segmentVisualName), std::move(startConnection), std::move(endConnection) });

		// Skip rest of line
		infoFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

// Load segment from fbx file using info from _segments[i], returns reference to loaded segment
// Just returns reference if already loaded
const Segment* SegmentHandler::loadSegment(unsigned i)
{
	// If already loaded, just return reference
	if (_segmentInfos[i].loadedIndex != -1)
	{
		return &_segments[_segmentInfos[i].loadedIndex];
	}
	// Else load Segment, then return
	else
	{
		// Load Segment from file
		_segments.push_back(Segment{
			_segmentInfos[i]._dataFileName,
			_segmentInfos[i]._visualFileName,
			_segmentInfos[i]._startConnection, 
			_segmentInfos[i]._endConnection		});

		// Set index in corresponding SegmentInfo
		_segmentInfos[i].loadedIndex = static_cast<int>(_segments.size()) - 1;

		return &_segments[_segmentInfos[i].loadedIndex];
	}
}