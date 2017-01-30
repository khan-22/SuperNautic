#pragma once
#ifndef SEGMENTHANDLER_H
#define SEGMENTHANDLER_H

#include <string>
#include <vector>

#include "Segment.hpp"

// Holds basic information about an uninstantiated track segment 
struct SegmentInfo
{
	std::string		_fileName;
	std::string		_startConnection;
	std::string		_endConnection;
	int loadedIndex { -1 }; // Index of loaded segment in _segments, -1 if not loaded

	SegmentInfo(std::string&& fileName, std::string&& startConnection, std::string&& endConnection)
	{
		_fileName = fileName;
		_startConnection = startConnection;
		_endConnection = endConnection;
	}
};

// Holds uninstantiated versions of track segments
class SegmentHandler
{
public:
	// Loads SegmentInfos from file with name fileName
	SegmentHandler(std::string fileName);


	// Returns reference to segment infos
	const std::vector<SegmentInfo>& infos() const
	{
		return _segmentInfos;
	}

	// Load segment from fbx file using info from _segments[i], returns reference to loaded segment
	// Just returns reference if already loaded
	const Segment* loadSegment(unsigned i);

private:
	// Segments that have been loaded from an fbx file
	std::vector<Segment>		_segments;

	// Basic info about all possible segments, loaded from a txt file
	std::vector<SegmentInfo>	_segmentInfos;
};

#endif // SEGMENTHANDLER_H