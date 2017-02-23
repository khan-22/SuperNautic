#include "Core/Track/SegmentInfo.hpp"

//Constructor
SegmentInfo::SegmentInfo(std::string&& dataFileName, std::string&& visualFileName, std::string&& windowFileName
	, char startConnection, char endConnection, int probability1, int probability2
	, int minInRow1, int minInRow2, int maxInRow1, int maxInRow2
	, int rotationOffset1, int rotationOffset2)
	: _dataFileName{ dataFileName }
	, _visualFileName{ visualFileName }
	, _windowFileName{ windowFileName }
	, _startConnection{ startConnection }
	, _endConnection{ endConnection }
	, _probability1{ probability1 }
	, _probability2{ probability2 }
	, _minInRow1{ minInRow1 }
	, _minInRow2{ minInRow2 }
	, _maxInRow1{ maxInRow1 }
	, _maxInRow2{ maxInRow2 }
	, _rotationOffset1{ rotationOffset1 }
	, _rotationOffset2{ rotationOffset2 }
{

}

//Destructor
SegmentInfo::~SegmentInfo()
{
	//Nothing
}

int SegmentInfo::getProbaility(const float p) const
{
	return int((1 - p) * _probability1 + p * _probability2);
}

int SegmentInfo::getMinInRow(const float p) const
{
	return int((1 - p) * _minInRow1 + p * _minInRow2);
}

int SegmentInfo::getMaxInRow(const float p) const
{
	return int((1 - p) * _maxInRow1 + p * _maxInRow2);
}

int SegmentInfo::getRotationOffset(const float p) const
{
	return int((1 - p) * _rotationOffset1 + p * _rotationOffset2);
}
