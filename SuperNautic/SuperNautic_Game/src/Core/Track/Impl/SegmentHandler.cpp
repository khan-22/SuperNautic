#include <fstream>

#include "../SegmentHandler.h"
#include "../../../Log.h"

// Loads SegmentInfos from file with name fileName
SegmentHandler::SegmentHandler(std::string fileName)
{
	std::ifstream infoFile {fileName};

	if (infoFile.bad())
	{
		// File could not be opened
		LOG_ERROR("Could not open SegmentInfo file ", fileName );
		return;
	}

	std::string segmentName;
	std::string startConnection;
	std::string endConnection;

	// Read SegmentInfos
	while (infoFile >> segmentName)
	{
		if (!(infoFile >> startConnection && infoFile >> endConnection))
		{
			// Wrong number of arguments
			LOG_ERROR("Incorrect input in SegmentInfo file ", fileName);
			return;
		}

		// Add read SegmentInfo to vector
		_segmentInfos.push_back(SegmentInfo{ std::move(segmentName), std::move(startConnection), std::move(endConnection) });
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
		// TODO
	}
}