#include <glm\gtx\transform.hpp>
#include <time.h>
#include <assert.h>

#include "Core/Track/Track.hpp"
#include "Core/Track/SegmentInfo.hpp"
#include "Core/Track/Segment.hpp"
#include "Core/Io/Log.hpp"
#include "GFX/Resources/Model.hpp"

// Default constructor (private)
Track::Track()
	: _endMargin(500)
{

}

// Real costructor
Track::Track(SegmentHandler * segmentHandler)
	: _endMargin(400)
{
	_segmentHandler = segmentHandler;
	//_endMatrix = glm::translate(glm::vec3(0, 0, 100));
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
void Track::setLength(const unsigned int length)
{
	assert(length >= 3000 && length <= 1000000);
	_targetLength = length;
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
	while (totalLength < 300)
	{
		bInsertNormalSegment(0, totalLength, false);
	}

	char endConnection = 'a';
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
			index = getIndex(endConnection);
			if (index == -1)
			{
				LOG_ERROR("WARNING! Something went wrong with the track generation! Not enough connections of type '", endConnection, "'?");
			}
		} while (index == prevIndex);
		// Normal segment placement
		if (index < _segmentHandler->infos().size())
		{
			// Randomize nr of same segment type in a row
			inRow = getInRow(index);

			for (unsigned int i = 0; i < inRow; i++)
			{
				//insertNormalSegment(index, totalLength, false);
				//insertStructure(0, totalLength);
				if (!bInsertNormalSegment(index, totalLength, true))
				{
					deleteSegments(totalLength, 300);
					_endMatrix = _track.back()->getModelMatrix() * _track.back()->getEndMatrix();
					break;
				}
			}
		}
		// Structures
		else
		{
			insertStructure(index - _segmentHandler->infos().size(), totalLength);
		}
		endConnection = _track.back()->getParent()->getEnd();
		prevIndex = _track.back()->getIndex();

		// Make the final stretch straight
		if (totalLength > _targetLength - _endMargin)
		{
			if (bEndTrack(totalLength))
			{
				break;
			}
		}
	}
	
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

// Returns a random index based on connection type
int Track::getIndex(char connectionType) const
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
	for (unsigned int i = 0; i < _segmentHandler->getNrOfStructures(); i++)
	{
		if (infos[_segmentHandler->getStructure(i)->pieces[0]->index]._startConnection == connectionType)
		{
			validSegments.push_back(infos.size() + i);
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
		if (validSegments[i] < infos.size())
		{
			totalProbability += infos[validSegments[i]]._probability;
		}
		else
		{
			totalProbability += _segmentHandler->getStructure(i - infos.size())->probability;
		}
	}
	// Randomizing and finding the corresponding segment
	int r = rand() % totalProbability;
	int tested = 0;
	for (unsigned int i = 0; i < validSegments.size(); i++)
	{
		if (validSegments[i] < infos.size())
		{

			int test = infos[validSegments[i]]._probability;
			if (r - tested < test)
			{
				//connectionType = infos[validSegments[i]]._endConnection;
				return i;
			}
			tested += test;
		}
		else
		{
			int test = _segmentHandler->getStructure(i - infos.size())->probability;
			if (r - tested < test)
			{
				return i;
			}
			tested += test;
		}
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

glm::vec3 Track::findForward(const glm::vec3 globalPosition, unsigned& segmentIndex, glm::vec3& returnPos)
{
	WaypointInfo closest;
	closest.found = false;
	unsigned closestIndex;

	// Check previous, current and next segment
	for (long i = static_cast<long>(segmentIndex) - 1; i <= static_cast<long>(segmentIndex) + 1; ++i)
	{
		// Make sure current segment is within bounds
		if (i >= 0 && i < _track.size())
		{
			WaypointInfo test = _track[i]->getParent()->findClosestWaypoint(glm::inverse(_track[i]->getModelMatrix()) * glm::vec4{globalPosition, 1.0f});

			// If test found an appropriate waypoint and closest is empty or test is closer than closest
			if (test.found && (!closest.found || test.distance < closest.distance))
			{
				closest = test;
				closestIndex = i;
			}
		}
	}

	if (!closest.found)
	{
		LOG_ERROR("No closest waypoint was found! Returning (0, 0, 1) as forward vector.");
		return glm::vec3{ 0, 0, 1 };
	}
	else
	{
		// Return to global world space
		closest.direction = _track[closestIndex]->getModelMatrix() * glm::vec4{ closest.direction, 0.0f };
		closest.position = _track[closestIndex]->getModelMatrix() * glm::vec4{ closest.position, 1.0f };

		// Update ship's current segment
		segmentIndex = closestIndex;
	}
	
	// Find direction and position of next waypoint
	WaypointInfo next = findNextWaypointInfo(closest, closestIndex);

	// Update ship return position
	returnPos = closest.position;

	if (bAlmostEqual(closest.direction, next.direction))
	{
		//return closest.direction;
	}

	// Project globalPosition onto the plane defined by closest.direction and closest.position
	// Cast a ray from this position in the direction of closest.direction and find 
	//     intersection with plane defined by next.direction and next.distance
	// The length of the ray is used to find interpolation value between waypoint normals
	glm::vec3 projectedGlobalPos{ globalPosition - glm::dot(closest.direction, globalPosition - closest.position) * closest.direction };

	// Distance from origin to next-plane
	float planeD = glm::dot(next.direction, next.position);

	float divisor = glm::dot(next.direction, closest.direction);

	// Ray is parallell to plane
	if (divisor <= 0.0f)
	{
		LOG_ERROR("The direction of two waypoints are orthogonal. Returning direction of waypoint ahead of position.");
		return next.direction;
	}

	float distanceToPlane = (-glm::dot(next.direction, projectedGlobalPos) + planeD) / divisor;

	// [0..1], 0 = at closest waypoint, 1 = at next waypoint
	float interpolationValue = glm::dot(closest.direction, globalPosition - closest.position) / distanceToPlane;

	// Interpolate between directions
	return glm::normalize(closest.direction * (1 - interpolationValue) + next.direction * interpolationValue);
}

// Inserts a segment with given index at the end of the track
bool Track::bInsertNormalSegment(const int index, int & length, bool testCollision)
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
	int maxRotOffset = segment->getRotationOffset() / angle;
	float rotVal = (rand() % (2 * maxRotOffset) - maxRotOffset) * angle;
	glm::mat4 rotMat = glm::rotate(glm::radians(rotVal), glm::vec3(0, 0, 1));
	_endMatrix = _endMatrix * modelEndMat * rotMat;
	length += segment->getLength();
	_track.push_back(tempInstance);
	return true;
}

// Inserts a whole pre-defined structure at the end of the track
void Track::insertStructure(const int index, int & length)
{
	int startLength = length;
	const SegmentHandler::Structure * s = _segmentHandler->getStructure(index);
	// Randomize how many times the structure should be looped
	int min = s->minInRow;
	int max = s->maxInRow;
	double scaled = (double)rand() / RAND_MAX;
	int amount = (max - min + 1) * scaled + min;
	// Determine and randomize if there should be "negative" rotation
	int rotationDir = 1;
	if (s->bAllowNegativRot)
	{
		if (rand() % 2 == 0)
		{
			rotationDir = -1;
		}
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
					deleteSegments(length, length - startLength);
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
			length += segment->getLength();
			_track.push_back(tempInstance);

			// Terminating if target length is approaching
			if (length > _targetLength - _endMargin)
			{
				return;
			}
		}
	}
}

// Deletes a certain length of the track (from the end)
void Track::deleteSegments(int & totalLength, const int lengthToDelete)
{
	int deletedLength = 0;
	while (deletedLength <= lengthToDelete && totalLength > 500)
	{
		int segmentLength = _track.back()->getLength();
		deletedLength += segmentLength;
		totalLength -= segmentLength;
		delete _track[_track.size() - 1];
		_track.erase(_track.begin() + _track.size() - 1);
	}
}

// Tries to end the track with a straight path
bool Track::bEndTrack(int & totalLength)
{
	while (totalLength < _targetLength)
	{
		if (!bInsertNormalSegment(0, totalLength, true))
		{
			deleteSegments(totalLength, _endMargin + 200);
			_endMatrix = _track.back()->getModelMatrix() * _track.back()->getEndMatrix();
			return false;
		}
	}
	return true;
}

// Render the track
void Track::render(GFX::DeferredRenderer& renderer, const int shipIndex)
{
	for (int i = -2; i < 30; i++)
	{
		int index = shipIndex + i;
		if (index >= 0 && index < _track.size())
		{
			renderer.render(*_track[index]);
		}
	}
}

WaypointInfo Track::findNextWaypointInfo(const WaypointInfo& current, unsigned segmentIndex) const
{
	WaypointInfo next;

	if (current.index >= _track[segmentIndex]->getParent()->getWaypoints().size() - 1)
	{
		// The waypoint is the last in its segment

		// If this is the last segment, just return direction of current
		if (segmentIndex >= _track.size() - 1)
		{
			return next = current;
		}
		// Else use first waypoint of next segment
		else
		{
			next.position = _track[segmentIndex + 1]->getModelMatrix() * glm::vec4{ _track[segmentIndex + 1]->getParent()->getWaypoints()[0], 1.0f };

			// Get local position, transform to global space, subtract next.position to get global direction, normalize
			next.direction = glm::normalize(
				glm::vec3{ (_track[segmentIndex + 1]->getModelMatrix() *
					glm::vec4{ _track[segmentIndex + 1]->getParent()->getWaypoints()[1], 1.0f }) }
				-next.position
			);
		}
	}
	else
	{
		// Use next waypoint in segment
		next.position = _track[segmentIndex]->getModelMatrix() * glm::vec4{ _track[segmentIndex]->getParent()->getWaypoints()[current.index + 1], 1.0f };

		if (current.index >= _track[segmentIndex]->getParent()->getWaypoints().size() - 2)
		{
			// Use the end of the segment to find direction of next

			// Get local position, transform to global space, subtract next.position to get global direction, normalize
			next.direction = glm::normalize(
				glm::vec3{ (_track[segmentIndex]->getModelMatrix() * _track[segmentIndex]->getEndMatrix() *
					glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f }) }
				-next.position
			);
		}
		else
		{
			// Use the waypoint after next to find direction of next

			// Get local position, transform to global space, subtract next.position to get global direction, normalize
			next.direction = glm::normalize(
				glm::vec3{ (_track[segmentIndex]->getModelMatrix() *
					glm::vec4{ _track[segmentIndex]->getParent()->getWaypoints()[current.index + 2], 1.0f }) }
				-next.position
			);
		}
	}

	return next;
}