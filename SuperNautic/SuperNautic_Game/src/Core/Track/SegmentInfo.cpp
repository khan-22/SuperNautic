#include "SegmentInfo.hpp"

//Constructor
SegmentInfo::SegmentInfo(std::string&& dataFileName, std::string&& visualFileName
	, char startConnection, char endConnection, int probability, int minInRow
	, int maxInRow, int rotationOffset)
	: _dataFileName{ dataFileName }
	, _visualFileName{ visualFileName }
	, _startConnection{ startConnection }
	, _endConnection{ endConnection }
	, _probability{ probability }
	, _minInRow{ minInRow }
	, _maxInRow{ maxInRow }
	, _rotationOffset{ rotationOffset }
{

}

//Destructor
SegmentInfo::~SegmentInfo()
{
	//Nothing
}