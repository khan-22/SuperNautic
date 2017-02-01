#include <glm\gtx\transform.hpp>
#include <time.h>

#include "Track.hpp"
#include "../../Log.hpp"

//Default constructor (private)
Track::Track()
{
	//Nothing
}

//Real costructor
Track::Track(SegmentHandler * segmentHandler)
{
	_segmentHandler = segmentHandler;
	_endMatrix = glm::rotate(180.f, glm::vec3(0.0, 1.0, 0.0));
}

//Destructor
Track::~Track()
{
	//Nothing
}

//Gets the targeted track length in whole meters
int Track::getTargetLength() const
{
	return _targetLength;
}

//Gets the generated track length in whole meters
int Track::getGeneratedLength() const
{
	return _generatedLength;
}

//Sets the track length in whole meters
bool Track::setLength(const int length)
{
	if (length >= 300 && length <= 100000)
	{
		_targetLength = length;
		return true;
	}
	return false;
}

//Set randomization seed
void Track::setSeed(const unsigned int seed)
{
	if (seed == 1)
	{
		srand(time(NULL));
	}
	else
	{
		_seed = seed;
		srand(_seed);
	}
}

//Generates the track
bool Track::generate()
{
	int totalLength = 0;
	//Create initial straight path
	const Segment * segment = _segmentHandler->loadSegment(0);
	for (unsigned int i = 0; i < 3; i++)
	{
		_track.push_back(SegmentInstance(segment, _endMatrix, true));
		_endMatrix = segment->getEndMatrix() * _endMatrix;
		totalLength += segment->getLength();
	}
	std::string end = "a";
	bool lighting = true;
	//Create random path
	while (totalLength < _targetLength)
	{
		int index = rand() % 4;
		const Segment * segment = _segmentHandler->loadSegment(index);
		_endMatrix = segment->getEndMatrix() * _endMatrix;
		SegmentInstance temp = SegmentInstance(segment, _endMatrix, lighting);
		_track.push_back(temp);
		totalLength += segment->getLength();
	}
	_generatedLength = totalLength;

	LOG("Track generated. Length: " + std::to_string(_generatedLength));
	return true;
}

