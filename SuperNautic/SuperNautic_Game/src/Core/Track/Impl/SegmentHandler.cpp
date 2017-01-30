#include <fstream>
#include <limits>

#include "../SegmentHandler.hpp"
#include "../../../Log.hpp"

// Loads SegmentInfos from file with name fileName
SegmentHandler::SegmentHandler(std::string path, std::string fileName) : _path{path}
{
	std::ifstream infoFile {_path + fileName};

	if (infoFile.bad())
	{
		// File could not be opened
		LOG_ERROR("Could not open SegmentInfo file ", path + fileName );
		return;
	}

	std::string segmentName;
	std::string startConnection;
	std::string endConnection;

	// Read SegmentInfos
	while (infoFile >> segmentName)
	{
		// blah_aa.fbx
		//      ^^
		// size - 6, size - 5 contains connection info
		startConnection = segmentName[segmentName.size() - 6];
		endConnection = segmentName[segmentName.size() - 5];

		// Add read SegmentInfo to vector
		_segmentInfos.push_back(SegmentInfo{ std::move(segmentName), std::move(startConnection), std::move(endConnection) });

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
		_segments.push_back(Segment{_path,
			_segmentInfos[i]._fileName, 
			_segmentInfos[i]._startConnection, 
			_segmentInfos[i]._endConnection		});

		// Set index in corresponding SegmentInfo
		_segmentInfos[i].loadedIndex = static_cast<int>(_segments.size()) - 1;

		return &_segments[_segmentInfos[i].loadedIndex];
	}
}