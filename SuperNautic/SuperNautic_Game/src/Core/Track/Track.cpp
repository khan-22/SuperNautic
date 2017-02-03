#include <glm\gtx\transform.hpp>
#include <time.h>

#include "Track.hpp"
#include "../../Log.hpp"
#include "SegmentInfo.hpp"

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
	char end = 'a';
	bool lighting = true;
	int prevIndex = -1;
	//Create random path
	while (totalLength < _targetLength)
	{
		//Randomize segment index
		int index;
		int inRow;
		do
		{
			index = getIndex(end);
		} while (index != prevIndex);
		prevIndex = index;
		//Randomize nr of same segment type in row
		inRow = getInRow(index);
		//Instanciate the segment(s)
		for (unsigned int i = 0; i < inRow; i++)
		{
			const Segment * segment = _segmentHandler->loadSegment(index);
			_endMatrix = segment->getEndMatrix() * _endMatrix;
			SegmentInstance temp = SegmentInstance(segment, _endMatrix, lighting);
			_track.push_back(temp);
			totalLength += segment->getLength();
		}
	}
	_generatedLength = totalLength;

	LOG("Track generated. Length: " + std::to_string(_generatedLength));
	return true;
}

int Track::getIndex(char connectionType) const
{
	std::vector<SegmentInfo> infos = _segmentHandler->infos();
	//Finding valid segments based on connection type
	std::vector<int> validSegments = std::vector<int>();
	for (unsigned int i = 0; i < infos.size(); i++)
	{
		if (infos[i]._startConnection == connectionType)
		{
			validSegments.push_back(i);
		}
	}
	//Calculating total probability
	unsigned int totalProbability = 0;
	for (unsigned int i = 0; i < validSegments.size(); i++)
	{
		totalProbability += infos[validSegments[i]]._probability;
	}
	//Randomizing and finding corresponding segment
	int r = rand() % totalProbability;
	int tested = 0;
	for (unsigned int i = 0; i < validSegments.size(); i++)
	{
		if (r - tested < infos[validSegments[i]]._probability)
		{
			return r - tested;
		}
		tested += infos[validSegments[i]]._probability;
	}
	return -1;
}

int Track::getInRow(int index) const
{
	std::vector<SegmentInfo> infos = _segmentHandler->infos();
	int min = infos[index]._minInRow;
	int max = infos[index]._maxInRow;
	double scaled = (double)rand() / RAND_MAX;
	return (max - min + 1) * scaled + min;
}
