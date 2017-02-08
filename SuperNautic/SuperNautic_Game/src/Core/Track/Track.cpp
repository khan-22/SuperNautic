#include <glm\gtx\transform.hpp>
#include <time.h>

#include "Track.hpp"
#include "../../Log.hpp"
#include "SegmentInfo.hpp"
#include "../../GFX/Model.hpp"

// Default constructor (private)
Track::Track()
{
	//Nothing
}

// Real costructor
Track::Track(SegmentHandler * segmentHandler)
{
	_segmentHandler = segmentHandler;
	_endMatrix = glm::mat4();
}

// Destructor
Track::~Track()
{
	for (unsigned int i = 0; i < _track.size(); i++)
	{
		delete _track[i];
	}
}

// Gets the targeted track length in whole meters
int Track::getTargetLength() const
{
	return _targetLength;
}

// Gets the generated track length in whole meters
int Track::getGeneratedLength() const
{
	return _generatedLength;
}

// Sets the track length in whole meters
bool Track::setLength(const int length)
{
	if (length >= 2000 && length <= 100000)
	{
		_targetLength = length;
		return true;
	}
	return false;
}

// Set randomization seed
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

// Generates the track
bool Track::generate()
{
	int totalLength = 0;
	// Make the inital stretch straight
	while (totalLength < 500)
	{
		insertSegment(0, 'a', totalLength, false);
	}
	char end = 'a';
	bool lighting = true;
	int prevIndex = -1;
	// Create random path
	while (true)
	{
		// Randomize segment index
		int index;
		int inRow;
		do
		{
			index = getIndex(end);
			if (index == -1)
			{
				LOG_ERROR("WARNING! There are not enough segments with connection type '", end, "'.");
			}
		} while (index == prevIndex);
		// Randomize nr of same segment type in row
		inRow = getInRow(index);
		// Instanciate the segment(s)
		bool collided = false;
		for (unsigned int i = 0; i < inRow; i++)
		{
			insertSegment(index, end, totalLength, false);
			/*if (!insertSegment(index, end, totalLength, true))
			{
				deleteSegments(totalLength, 200);
				_endMatrix = _track[_track.size() - 1]->getEndMatrix();
				collided = true;
				break;
			}*/
		}
		if (!collided)
		{
			prevIndex = index;
		}
		if (totalLength > _targetLength - 500 && end == 'a')
			break;
	}
	// Make the final stretch straight
	while (totalLength < _targetLength)
	{
		insertSegment(0, 'a', totalLength, false);
	}
	_generatedLength = totalLength;

	LOG("Track generated. Length: " + std::to_string(_generatedLength) + " meters.");
	return true;
}

int Track::getNrOfSegments() const
{
	return _track.size();
}

SegmentInstance& Track::getInstance(int index)
{
	return *_track[index];
}

// Returns a random index based on connection type
int Track::getIndex(char & connectionType) const
{
	std::vector<SegmentInfo> infos = _segmentHandler->infos();
	// Finding valid segments based on connection type
	std::vector<int> validSegments = std::vector<int>();
	for (unsigned int i = 0; i < infos.size(); i++)
	{
		if (infos[i]._startConnection == connectionType)
		{
			validSegments.push_back(i);
		}
	}
	if (validSegments.size() < 2)
	{
		return -1;
	}
	// Calculating total probability
	unsigned int totalProbability = 0;
	for (unsigned int i = 0; i < validSegments.size(); i++)
	{
		totalProbability += infos[validSegments[i]]._probability;
	}
	// Randomizing and finding corresponding segment
	int r = rand() % totalProbability;
	int tested = 0;
	for (unsigned int i = 0; i < validSegments.size(); i++)
	{
		int test = infos[validSegments[i]]._probability;
		if (r - tested < test)
		{
			connectionType = infos[validSegments[i]]._endConnection;
			return i;
		}
		tested += test;
	}
	return -1;
}

// Returns the random nr of same segment in a row
int Track::getInRow(int index) const
{
	std::vector<SegmentInfo> infos = _segmentHandler->infos();
	int min = infos[index]._minInRow;
	int max = infos[index]._maxInRow;
	double scaled = (double)rand() / RAND_MAX;
	return (max - min + 1) * scaled + min;
}

// Inserts a segment with given index at the end of the track
bool Track::insertSegment(const int index, const char connection, int & length, bool testCollision)
{
	const Segment * segment = _segmentHandler->loadSegment(index);
	SegmentInstance* tempInstance = new SegmentInstance(segment, _endMatrix, true);
	if (testCollision)
	{
		for (unsigned int i = 0; i < _track.size() - 2; i++)
		{
			if (tempInstance->bTestCollision(*_track[i]))
			{
				delete tempInstance;
				return false;
			}
		}
	}
	glm::mat4 modelEndMat = segment->getEndMatrix();
	int angle = 360.f / _segmentHandler->getConnectionRotation(connection);
	int maxRotOffset = segment->getRotationOffset() / angle;
	float rotVal = (rand() % (2 * maxRotOffset) - maxRotOffset) * angle;
	glm::mat4 rotMat = glm::rotate(glm::radians(rotVal), glm::vec3(0, 0, 1));
	_endMatrix = _endMatrix * modelEndMat * rotMat;
	length += segment->getLength();
	_track.push_back(tempInstance);
	return true;
}

void Track::deleteSegments(int & totalLength, const int lengthToDelete)
{
	int deletedLength = 0;
	while (deletedLength <= lengthToDelete && totalLength > 300)
	{
		int segmentLength = _track[_track.size() - 1]->getLength();
		deletedLength += segmentLength;
		totalLength -= segmentLength;
		delete _track[_track.size() - 1];
		_track.erase(_track.begin() + _track.size() - 1);
	}
}
