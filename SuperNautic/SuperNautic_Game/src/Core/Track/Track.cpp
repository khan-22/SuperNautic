#include <glm\gtx\transform.hpp>
#include <time.h>
#include <assert.h>

#include "Core/Track/Track.hpp"
#include "Core/Track/SegmentInfo.hpp"
#include "Core/Track/Segment.hpp"
#include "Core/Io/Log.hpp"
#include "GFX/Resources/Model.hpp"

// Real costructor
Track::Track(SegmentHandler * segmentHandler)
	: _segmentHandler(segmentHandler)
	, _seed(1)
	, _curviness(0)
	, _targetLength(10000)
	, _generatedLength(0)
	, _totalProgress(0.f)
	, _endMargin(400)
	, _endMatrix(glm::mat4())
{
	
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
void Track::setLength(const unsigned int length)
{
	assert(length >= 3000 && length <= 1000000);
	_targetLength = length;
	_progressionLength = _targetLength / 30;
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

// Sets the difficulty of the track (0-5)
void Track::setCurviness(const unsigned int curviness)
{
	assert(curviness >= 0 && curviness <= 5);
	_curviness = curviness / 5.f;
}

void Track::startNewTrack()
{
	_generatedLength = 0;
	_totalProgress = 0.f;
	_endMatrix = glm::mat4();
	_endConnection = 'a';
	_prevIndex = -1;
	_lastSegment = nullptr;
	for (unsigned int i = 0; i < _track.size(); i++)
	{
		delete _track[i];
	}
}

// Generates the track
bool Track::bGenerate()
{
	// Make the inital stretch straight
	while (_generatedLength < 300)
	{
		bInsertNormalSegment(0, false);
	}

	// Create random path
	while (_generatedLength - _lengthAfterLastCall < _progressionLength)
	{
		// Randomize segment index
		int index;
		int inRow;
		index = getIndex();
		// Normal segment placement
		if (index < _segmentHandler->infos().size())
		{
			// Randomize nr of same segment type in a row
			inRow = getInRow(index);

			for (unsigned int i = 0; i < inRow; i++)
			{
				if (!bInsertNormalSegment(index, true))
				{
					deleteSegments(300);
					_endMatrix = _track.back()->getModelMatrix() * _track.back()->getEndMatrix();
					break;
				}
			}
		}
		// Structures
		else
		{
			insertStructure(index - _segmentHandler->infos().size());
		}
		_endConnection = _track.back()->getParent()->getEnd();
		_prevIndex = index;
		_lastSegment = _track.back()->getParent();

		// Make the final stretch straight
		if (_generatedLength > _targetLength - _endMargin)
		{
			if (bEndTrack())
			{
				return true;
				LOG("Track generated. Length: ", _generatedLength);
			}
			else
			{
				_endConnection = _track.back()->getParent()->getEnd();
				_prevIndex = index;
				_lastSegment = _track.back()->getParent();
			}
		}
	}

	_lengthAfterLastCall = _generatedLength;
	_totalProgress = (float)_generatedLength / _targetLength;
	LOG("Progression: ", _totalProgress * 100);
	return false;
}

int Track::getNrOfSegments() const
{
	return _track.size();
}

SegmentInstance* Track::getInstance(int index)
{
	return _track[index];
}

float Track::getProgression() const
{
	return _totalProgress;
}

// Returns a random index based on connection type
int Track::getIndex() const
{
	std::vector<SegmentInfo> infos = _segmentHandler->infos();
	int nonZeroProbSegments = 0;
	for (unsigned int i = 0; i < infos.size(); i++)
	{
		if (infos[i].getProbaility(_curviness) != 0)
		{
			nonZeroProbSegments++;
		}
	}
	if (nonZeroProbSegments >= 2)
	{
		// Finding valid segments based on connection type
		std::vector<int> validSegments = std::vector<int>();
		// Normal segments
		for (unsigned int i = 0; i < infos.size(); i++)
		{
			if (infos[i]._startConnection == _endConnection && infos[i].getProbaility(_curviness) != 0
				&& _segmentHandler->loadSegment(i) != _lastSegment)
			{
				validSegments.push_back(i);
			}
		}
		// Structures
		for (unsigned int i = 0; i < _segmentHandler->getNrOfStructures(); i++)
		{
			if (infos[_segmentHandler->getStructure(i)->pieces[0]->index]._startConnection == _endConnection
				&& i != _prevIndex && _segmentHandler->getStructure(i)->curviness <= _curviness)
			{
				validSegments.push_back(infos.size() + i);
			}
		}
		// Calculating total probability
		unsigned int totalProbability = 0;
		for (unsigned int i = 0; i < validSegments.size(); i++)
		{
			if (validSegments[i] < infos.size())
			{
				totalProbability += infos[validSegments[i]].getProbaility(_curviness);
			}
			else
			{
				int index = validSegments[i] - infos.size();
				int prob = _segmentHandler->getStructure(index)->getProbability(_curviness);
				totalProbability += prob;
			}
		}
		// Randomizing and finding the corresponding segment
		int r = rand() % totalProbability;
		int tested = 0;
		for (unsigned int i = 0; i < validSegments.size(); i++)
		{
			if (validSegments[i] < infos.size())
			{
				int test = infos[validSegments[i]].getProbaility(_curviness);
				if (r - tested < test)
				{
					return validSegments[i];
				}
				tested += test;
			}
			else
			{
				int test = _segmentHandler->getStructure(validSegments[i] - infos.size())->getProbability(_curviness);
				if (r - tested < test)
				{
					return i;
				}
				tested += test;
			}
		}
	}
	else
	{
		return 0;
	}
	
	//This should never ever run!
	assert(true);
}

// Returns the random nr of same segment in a row
int Track::getInRow(int index) const
{
	std::vector<SegmentInfo> infos = _segmentHandler->infos();
	int min = infos[index].getMinInRow(_curviness);
	int max = infos[index].getMaxInRow(_curviness);
	double scaled = (double)rand() / RAND_MAX;
	return (max - min + 1) * scaled + min;
}

glm::vec3 Track::findForward(const glm::vec3 globalPosition, unsigned& segmentIndex, glm::vec3& returnPos)
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
	float dist = glm::dot(glm::normalize(betweenWaypoints), (globalPosition - behindPos)) / glm::length(betweenWaypoints);

	// Set return pos
	returnPos = behindPos;

	// Find forward vector, change to proper rotation?
	return glm::vec3{ glm::normalize(behindDir * (1.0f - dist) + aheadDir * dist) };
}

// Inserts a segment with given index at the end of the track
bool Track::bInsertNormalSegment(const int index, bool testCollision)
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
	int angle = 360.f / _segmentHandler->getConnectionRotation(segment->getStart());
	int maxRotOffset = segment->getInfo()->getRotationOffset(_curviness) / angle;
	float rotVal = (rand() % (2 * maxRotOffset) - maxRotOffset) * angle;
	glm::mat4 rotMat = glm::rotate(glm::radians(rotVal), glm::vec3(0, 0, 1));
	_endMatrix = _endMatrix * modelEndMat * rotMat;
	_generatedLength += segment->getLength();
	_track.push_back(tempInstance);
	return true;
}

// Inserts a whole pre-defined structure at the end of the track
void Track::insertStructure(const int index)
{
	int startLength = _generatedLength;
	const SegmentHandler::Structure * s = _segmentHandler->getStructure(index);
	// Randomize how many times the structure should be looped
	int min = s->minInRow;
	int max = s->maxInRow;
	double scaled = (double)rand() / RAND_MAX;
	int amount = (max - min + 1) * scaled + min;
	// Randomize if there should be "negative" rotation
	int rotationDir = 1;
	if (rand() % 2 == 0)
	{
		rotationDir = -1;
	}
	// Amount of "loops"
	for (unsigned int i = 0; i < amount; i++)
	{
		// Amount of pieces in each "loop"
		for (unsigned int j = 0; j < s->pieces.size(); j++)
		{
			const SegmentHandler::StructurePiece * p = s->pieces[j];
			const Segment * segment = _segmentHandler->loadSegment(p->index);
			SegmentInstance* tempInstance = new SegmentInstance(segment, _endMatrix, true);
			for (unsigned int i = 0; i < _track.size() - 2; i++)
			{
				if (tempInstance->bTestCollision(*_track[i]))
				{
					delete tempInstance;
					deleteSegments(_generatedLength - startLength);
					_endMatrix = _track.back()->getModelMatrix() * _track.back()->getEndMatrix();
					return;
				}
			}
			glm::mat4 modelEndMat = segment->getEndMatrix();
			int angle = 360.f / _segmentHandler->getConnectionRotation(segment->getStart());
			// Randomize angle from structure info
			int minRot = p->minRotation / angle;
			int maxRot = p->maxRotation / angle;
			double scaled = (double)rand() / RAND_MAX;
			float rotVal = ((maxRot - minRot) * scaled + minRot) * angle;
			// Finalizing
			glm::mat4 rotMat = glm::rotate(glm::radians(rotVal * rotationDir), glm::vec3(0, 0, 1));
			_endMatrix = _endMatrix * modelEndMat * rotMat;
			_generatedLength += segment->getLength();
			_track.push_back(tempInstance);

			// Terminating if target length is approaching
			if (_generatedLength > _targetLength - _endMargin)
			{
				return;
			}
		}
	}
}

// Deletes a certain length of the track (from the end)
void Track::deleteSegments(const int lengthToDelete)
{
	int deletedLength = 0;
	while (deletedLength <= lengthToDelete && _generatedLength > 500)
	{
		int segmentLength = _track.back()->getLength();
		deletedLength += segmentLength;
		_generatedLength -= segmentLength;
		delete _track[_track.size() - 1];
		_track.erase(_track.begin() + _track.size() - 1);
	}
}

// Tries to end the track with a straight path
bool Track::bEndTrack()
{
	while (_generatedLength < _targetLength)
	{
		if (!bInsertNormalSegment(0, true))
		{
			deleteSegments(_endMargin + 200);
			_endMatrix = _track.back()->getModelMatrix() * _track.back()->getEndMatrix();
			return false;
		}
	}
	return true;
}

// Render the track
void Track::render(GFX::DeferredRenderer& renderer, const int shipIndex)
{
	for (int i = -2; i < 300; i++)
	{
		int index = shipIndex + i;
		if (index >= 0 && index < _track.size())
		{
			renderer.render(*_track[index]);
		}
	}
}
