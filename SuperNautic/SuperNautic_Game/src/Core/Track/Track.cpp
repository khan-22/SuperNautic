#include <glm\gtx\transform.hpp>
#include <time.h>

#include "Track.hpp"
#include "../../Log.hpp"
#include "SegmentInfo.hpp"
#include "../../GFX/Model.hpp"
#include "Segment.hpp"

//Default constructor (private)
Track::Track()
{
	//Nothing
}

//Real costructor
Track::Track(SegmentHandler * segmentHandler)
{
	_segmentHandler = segmentHandler;
	_endMatrix = glm::mat4();
}

//Destructor
Track::~Track()
{
	for (unsigned int i = 0; i < _track.size(); i++)
	{
		delete _track[i];
	}
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
	for (unsigned int i = 0; i < 10; i++)
	{
		for (unsigned int i = 0; i < 5; i++)
		{
			const Segment * segment = _segmentHandler->loadSegment(0);
			_track.push_back(new SegmentInstance(segment, _endMatrix, true));
			glm::mat4 temp = segment->getEndMatrix();
			glm::mat4 rot = glm::rotate(0.f, glm::vec3(0, 0, 1));
			_endMatrix = _endMatrix * temp * rot;
			totalLength += segment->getLength();
		}
		for (unsigned int i = 0; i < 5; i++)
		{
			const Segment * segment = _segmentHandler->loadSegment(2);
			_track.push_back(new SegmentInstance(segment, _endMatrix, true));
			glm::mat4 temp = segment->getEndMatrix();
			glm::mat4 rot = glm::rotate(0.5f, glm::vec3(0, 0, 1));
			_endMatrix = _endMatrix * temp * rot;
			totalLength += segment->getLength();
		}
	}
	char end = 'a';
	bool lighting = true;
	int prevIndex = -1;
	//Create random path
	//while (totalLength < _targetLength)
	//{
	//	//Randomize segment index
	//	int index;
	//	int inRow;
	//	do
	//	{
	//		index = getIndex(end);
	//	} while (index != prevIndex);
	//	prevIndex = index;
	//	//Randomize nr of same segment type in row
	//	inRow = getInRow(index);
	//	//Instanciate the segment(s)
	//	for (unsigned int i = 0; i < inRow; i++)
	//	{
	//		const Segment * segment = _segmentHandler->loadSegment(index);
	//		_endMatrix = segment->getEndMatrix() * _endMatrix;
	//		SegmentInstance temp = SegmentInstance(segment, _endMatrix, lighting);
	//		_track.push_back(temp);
	//		totalLength += segment->getLength();
	//	}
	//}
	_generatedLength = totalLength;

	LOG("Track generated. Length: " + std::to_string(_generatedLength) + " meters.");
	return true;
}

int Track::getNrOfSegments() const
{
	return _track.size();
}

SegmentInstance* Track::getInstance(int index)
{
	return _track[index];
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

glm::vec3 Track::findForward(const glm::vec3 globalPosition, unsigned& segmentIndex)
{
						 // Waypoints,	segment index
	std::vector<std::pair<WaypointInfo, long>> closestWaypoints;

	// Check previous, current and next segment
	for (long i = static_cast<long>(segmentIndex) - 1; i <= static_cast<long>(segmentIndex) + 1; ++i)
	{
		// Make sure current segment is within bounds
		if (i >= 0 && i < _track.size())
		{
			std::pair<WaypointInfo, WaypointInfo> wps = _track[i]->getParent()->findClosestWaypoints(glm::inverse(_track[i]->getModelMatrix()) * glm::vec4{globalPosition, 1.0f});

			closestWaypoints.push_back(std::make_pair(wps.first, i));
			closestWaypoints.push_back(std::make_pair(wps.second, i));
		}
	}

	// Find the two neighboring waypoints closest to ship, these will be just ahead and behind ship
	// closestWaypoints are always sorted in order of appearance in track, because Segment::findClosestWaypoints() returns them in that order
	float lowestSum = closestWaypoints[0].first.distance + closestWaypoints[1].first.distance;
	unsigned lowestIndex = 0;
	for (unsigned i = 1; i < closestWaypoints.size() - 1; ++i)
	{
		float testSum = closestWaypoints[i].first.distance + closestWaypoints[i + 1].first.distance;
		if (testSum < lowestSum)
		{
			lowestSum = testSum;
			lowestIndex = i;
		}
	}

	// Update segment index of ship
	segmentIndex = static_cast<unsigned>(closestWaypoints[lowestIndex].second);

	// Transform vectors to global space
	// Waypoint behind ship
	glm::vec3 behindPos = _track[closestWaypoints[lowestIndex].second]->getModelMatrix() * glm::vec4{ closestWaypoints[lowestIndex].first.position, 1.0f };
	glm::vec3 behindDir = _track[closestWaypoints[lowestIndex].second]->getModelMatrix() * glm::vec4{ closestWaypoints[lowestIndex].first.direction, 0.0f };

	// Waypoint ahead of ship
	glm::vec3 aheadPos = _track[closestWaypoints[lowestIndex + 1].second]->getModelMatrix() * glm::vec4{ closestWaypoints[lowestIndex + 1].first.position, 1.0f };
	glm::vec3 aheadDir = _track[closestWaypoints[lowestIndex + 1].second]->getModelMatrix() * glm::vec4{ closestWaypoints[lowestIndex + 1].first.direction, 0.0f };

	// Vector from behind waypoint to ahead waypoint
	glm::vec3 betweenWaypoints = aheadPos - behindPos;

	// Find [0..1], 0 = ship is at behind waypoint, 1 = ship is at ahead waypoint
	float dist = glm::dot(glm::normalize(betweenWaypoints), globalPosition) / glm::length(betweenWaypoints);
	
	// Find forward vector, change to proper rotation?
	return glm::vec3{ glm::normalize(behindDir * (1.0f - dist) + aheadDir * dist) };
}

void Track::render(GFX::DeferredRenderer& renderer)
{
	for (unsigned i = 0; i < _track.size(); ++i)
	{
		renderer.render(*_track[i]);
	}
}