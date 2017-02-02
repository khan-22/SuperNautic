#pragma once
#ifndef SEGMENTINFO_HPP
#define SEGMENTINFO_HPP

#include <string>

// Holds basic information about an uninstantiated track segment 
class SegmentInfo
{
public:
	SegmentInfo(std::string&& dataFileName, std::string&& visualFileName
		, char startConnection, char endConnection, int probability, int minInRow
		, int maxInRow, int rotationOffset);
	~SegmentInfo();

	std::string		_dataFileName;
	std::string		_visualFileName;
	char			_startConnection;
	char			_endConnection;
	int				_probability;
	int				_minInRow;
	int				_maxInRow;
	int				_rotationOffset;
	int loadedIndex{ -1 }; // Index of loaded segment in _segments, -1 if not loaded
};

#endif // !SEGMENTINFO_HPP
