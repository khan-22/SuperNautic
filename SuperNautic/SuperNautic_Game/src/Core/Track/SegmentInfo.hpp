#pragma once
#ifndef SEGMENTINFO_HPP
#define SEGMENTINFO_HPP

#include <string>

// Holds basic information about an uninstantiated track segment 
class SegmentInfo
{
public:
	SegmentInfo(std::string&& dataFileName, std::string&& visualFileName, std::string&& windowFileName
		, char startConnection, char endConnection, int probability1, int probability2
		, int minInRow1, int minInRow2, int maxInRow1, int maxInRow2
		, int rotationOffset1, int rotationOffset2);
	~SegmentInfo();
	int getProbaility(const float p) const;
	int getMinInRow(const float p) const;
	int getMaxInRow(const float p) const;
	int getRotationOffset(const float p) const;

	std::string		_dataFileName;
	std::string		_visualFileName;
	std::string		_windowFileName;
	char			_startConnection;
	char			_endConnection;
	int				_probability1;
	int				_probability2;
	int				_minInRow1;
	int				_minInRow2;
	int				_maxInRow1;
	int				_maxInRow2;
	int				_rotationOffset1;
	int				_rotationOffset2;
	int loadedIndex{ -1 }; // Index of loaded segment in _segments, -1 if not loaded
};

#endif // !SEGMENTINFO_HPP
