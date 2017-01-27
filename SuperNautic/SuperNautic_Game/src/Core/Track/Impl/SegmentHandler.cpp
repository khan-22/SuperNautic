#include <fstream>

#include "../SegmentHandler.h"
#include "../../../Log.h"

// Loads SegmentInfos from file with name fileName
SegmentHandler::SegmentHandler(std::string fileName)
{
	std::ifstream infoFile { fileName };

	if (infoFile.fail())
	{
		// File could not be opened
		LOG_ERROR("Could not open SegmentInfo file ", fileName );
	}

	std::string segmentName;
	std::string startConnection;
	std::string endConnection;

	// Read SegmentInfos
	try
	{
		while (infoFile >> segmentName)
		{
			infoFile >> startConnection;
			infoFile >> endConnection;

			_segmentInfos.push_back(SegmentInfo{ std::move(segmentName), std::move(startConnection), std::move(endConnection) });
		}
	}
	catch (std::ios_base::failure e)
	{
		LOG_ERROR(e.what());
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