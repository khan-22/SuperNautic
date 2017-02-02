#pragma once
#ifndef SEGMENTHANDLER_H
#define SEGMENTHANDLER_H

#include <string>
#include <vector>

#include "Segment.hpp"




// Holds uninstantiated versions of track segments
class SegmentHandler
{
public:
	// Loads SegmentInfos from file with name fileName in path
	SegmentHandler(std::string filePath);


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

	// Path to res\models
	static std::string basePath;
};

#endif // SEGMENTHANDLER_H