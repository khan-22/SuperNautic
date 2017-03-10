#include <glm\gtx\transform.hpp>
#include <time.h>
#include <assert.h>
#include <functional>
#include <glm/gtx/transform.hpp>

#include "Core/Track/Track.hpp"
#include "Core/Track/SegmentInfo.hpp"
#include "Core/Track/Segment.hpp"
#include "Core/Io/Log.hpp"
#include "GFX/Resources/Model.hpp"

const unsigned int Track::_MIN_LENGTH = 3000;
const unsigned int Track::_MAX_LENGTH = 1000000;

const unsigned int Track::_MIN_CURVINESS = 0;
const unsigned int Track::_MAX_CURVINESS = 5;

const unsigned int Track::_MIN_DIFFICULTY = 0;
const unsigned int Track::_MAX_DIFFICULTY = 5;


// Real costructor
Track::Track(SegmentHandler * segmentHandler, ObstacleHandler * obstacleHandler)
	: _segmentHandler(segmentHandler)
	, _obstacleHandler(obstacleHandler)
	, _seed("1")
	, _curviness(0.4f)
	, _difficulty(0.6f)
	, _targetLength(10000.f)
	, _generatedLength(0.f)
	, _totalProgress(0.f)
	, _progressionLength(1500.f)
	, _endMargin(600.f)
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
	return static_cast<int>(_targetLength);
}

// Gets the generated track length in whole meters
int Track::getGeneratedLength() const
{
	return static_cast<int>(_generatedLength);
}

unsigned int Track::getCurviness() const
{
    return static_cast<unsigned int>(_MAX_CURVINESS * _curviness);
}

unsigned int Track::getDifficulty() const
{
    return static_cast<unsigned int>(_MAX_DIFFICULTY * _difficulty);
}


// Sets the track length in whole meters
void Track::setLength(const unsigned int length)
{
	assert(length >= _MIN_LENGTH && length <= _MAX_LENGTH);
	_targetLength = static_cast<float>(length);
}

// Set randomization seed
void Track::setSeed(const std::string& seed)
{
	_seed = seed;
	if (seed.length() == 1 && seed[0] == '1')
	{
		srand(static_cast<unsigned int>(time(NULL)));
	}
	else
	{
		unsigned int intSeed = 0;
		for (unsigned int i = 0; i < seed.length(); i++)
		{
			intSeed += (unsigned int)((seed[i] - 65) * static_cast<unsigned int>(powf(10, static_cast<float>(i))));
		}
		srand(intSeed);
	}
}

// Get the seed in string format
const std::string & Track::getSeed() const
{
	return _seed;
}

// Sets the curviness of the track (0-5)
void Track::setCurviness(const unsigned int curviness)
{
	assert(curviness >= _MIN_CURVINESS && curviness <= _MAX_CURVINESS);
	_curviness = curviness / float(_MAX_CURVINESS);
}

// Sets the difficulty of the track (0-5)
void Track::setDifficulty(const unsigned int difficulty)
{
	assert(difficulty >= _MIN_DIFFICULTY && difficulty <= _MAX_DIFFICULTY);
	_difficulty = difficulty / float(_MAX_DIFFICULTY);
}

// Resets the track
void Track::startNewTrack()
{
	_generatedLength = 0.f;
	_totalProgress = 0.f;
	_endMatrix = glm::mat4();
	_endConnection = 'a';
	_prevIndex = -1;
	_lastSegment = nullptr;
	_lengthAfterLastCall = 0.f;
	_lengthWithCurrentConnectionType = 0.f;
	for (unsigned int i = 0; i < _track.size(); i++)
	{
		delete _track[i];
	}
	_track.clear();
	_segmentWindows.clear();
	_temperatureZones.clear();

	_octree.reset(new Octree<SegmentInstance*>(glm::vec3(0.f, 0.f, 0.f), _targetLength * 1.1f + 1000.f, 2000.f, 5));

	_darkAreas.clear();
	_darkAreaPlayerValues.clear();
}

// Needs to be called before launching a new track
void Track::setNrOfPlayers(unsigned int nrOfPlayers)
{
	_darkAreaPlayerValues.resize(nrOfPlayers, { 0.f, -1, 0 });
}

// Generates the track
bool Track::bGenerate()
{
	// Make the inital stretch straight
	while (_generatedLength < 400.f)
	{
		bInsertNormalSegment(0, true);
	}

	float failedRecently = 0.f;
	// Create random path
	while (_generatedLength - _lengthAfterLastCall < _progressionLength)
	{
		// Randomize segment index
		int index;
		unsigned int inRow;
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
					if (failedRecently > 0.1f)
					{
						failedRecently += 400.f;
					}
					failedRecently += 500.f;
					deleteSegments(failedRecently);
					_endMatrix = _track.back()->getModelMatrix() * _track.back()->getEndMatrix();
					break;
				}
				else
				{
					if (failedRecently > 0.1f)
					{
						failedRecently -= _lastSegment->getLength();
						if (failedRecently < 0.f)
						{
							failedRecently = 0.f;
						}
					}
				}
			}
		}
		// Structures
		else
		{
			insertStructure(index - 100);
		}
		char newConnection = _track.back()->getParent()->getEnd();
		if (newConnection != _endConnection)
		{
			_lengthWithCurrentConnectionType = 0.f;
		}
		else
		{
			_lengthWithCurrentConnectionType += _track.back()->getParent()->getLength();
		}
		_endConnection = newConnection;
		_prevIndex = index;
		_lastSegment = _track.back()->getParent();

		// Make the final stretch straight
		if (_generatedLength > _targetLength - _endMargin)
		{
			if (bEndTrack())
			{
				if (_difficulty > 0.05f)
				{
					placeObstacles();
				}
				placeDarkAreas();
				// End segment
				bInsertNormalSegment(static_cast<int>(_segmentHandler->infos().size()) - 1, true);
				bInsertNormalSegment(0, true);

				LOG("Track generated. Length: ", _generatedLength);
				return true;
			}
			else
			{
				failedRecently += 400.f;
				_endMatrix = _track.back()->getModelMatrix() * _track.back()->getEndMatrix();
				char newConnection = _track.back()->getParent()->getEnd();
				if (newConnection != _endConnection)
				{
					_lengthWithCurrentConnectionType = 0.f;
				}
				else
				{
					_lengthWithCurrentConnectionType += _track.back()->getParent()->getLength();
				}
				_endConnection = newConnection;
				_prevIndex = index;
				_lastSegment = _track.back()->getParent();
			}
		}
	}

	_lengthAfterLastCall = _generatedLength;
	_totalProgress = _generatedLength / _targetLength;
	return false;
}

// Get the number of segment instances the track is made of
int Track::getNrOfSegments() const
{
	return static_cast<int>(_track.size());
}

// Get a instance-pointer based on index in the track
SegmentInstance* Track::getInstance(int index)
{
	return _track[index];
}

// Get the track progression in percent [0-1];
float Track::getProgression() const
{
	return _totalProgress;
}

// Returns a random index based on connection type
int Track::getIndex() const
{
	std::vector<SegmentInfo> infos = _segmentHandler->infos();
	// Finding valid segments based on connection type
	std::vector<unsigned int> validSegments = std::vector<unsigned int>();
	// Normal segments
	for (unsigned int i = 0; i < infos.size(); i++)
	{
		if (infos[i]._startConnection == _endConnection
			&& infos[i].getProbability(_curviness) != 0)
		{
			if (_segmentHandler->getNrOfSegmentsOfType(_endConnection, _difficulty) > 2)
			{
				if ((infos[i]._endConnection != _endConnection && _lengthWithCurrentConnectionType >= 600.f)
					|| infos[i]._endConnection == _endConnection
					&& _segmentHandler->loadSegment(i) != _lastSegment)
				{
					validSegments.push_back(i);
				}
			}
			else
			{
				validSegments.push_back(i);
			}
		}
	}
	// Structures
	unsigned int amount = _segmentHandler->getNrOfStructures();
	for (unsigned int i = 0; i < amount; i++)
	{
		unsigned int index = _segmentHandler->getStructure(i)->pieces[0]->index;
		if (infos[index]._startConnection == _endConnection
			&& index != _prevIndex && _segmentHandler->getStructure(i)->curviness <= _curviness)
		{
			validSegments.push_back(static_cast<unsigned int>(100) + i);
		}
	}
	// Calculating total probability
	unsigned int totalProbability = 0;
	for (unsigned int i = 0; i < validSegments.size(); i++)
	{
		if (validSegments[i] < infos.size())
		{
			totalProbability += infos[validSegments[i]].getProbability(_curviness);
		}
		else
		{
			unsigned int index = validSegments[i] - 100;
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
			int test = infos[validSegments[i]].getProbability(_curviness);
			if (r - tested < test)
			{
				return validSegments[i];
			}
			tested += test;
		}
		else
		{
			int test = _segmentHandler->getStructure(validSegments[i] - 100)->getProbability(_curviness);
			if (r - tested < test)
			{
				return validSegments[i];
			}
			tested += test;
		}
	}

	//This should never ever run!
	assert(true);

	return -1; // Suppress compiler warning
}

// Returns the random nr of same segment in a row
int Track::getInRow(int index) const
{
	std::vector<SegmentInfo> infos = _segmentHandler->infos();
	int min = infos[index].getMinInRow(_curviness);
	int max = infos[index].getMaxInRow(_curviness);
	double scaled = (double)rand() / RAND_MAX;
	return static_cast<int>((max - min + 1) * scaled) + min;
}


glm::vec3 Track::findForward(const glm::vec3 globalPosition, unsigned int& segmentIndex, glm::vec3& returnPos, float& lengthInSegment, glm::vec3& directionDifference)
{
	WaypointInfo closest;
	closest.found = false;
	unsigned int closestIndex;

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
	if (distanceToPlane <= 0.0f)	// Protect against the case where the track runs out of waypoints ahead of the player
	{
		interpolationValue = 1.0f;
	}

	// Calculate current length in segment
	lengthInSegment = 0.0f;

	// Add previous waypoint lengths
	for (unsigned int i = 0; i < closest.index; ++i)
	{
		lengthInSegment += glm::distance(_track[closestIndex]->getParent()->getWaypoints()[i], _track[closestIndex]->getParent()->getWaypoints()[i + 1]);
	}

	// Add current length
	lengthInSegment += glm::dot(closest.direction, globalPosition - closest.position);

	directionDifference = closest.direction - next.direction;

	// Interpolate between directions
	return glm::normalize(closest.direction * (1 - interpolationValue) + next.direction * interpolationValue);
}

// Inserts a segment with given index at the end of the track
bool Track::bInsertNormalSegment(const int index, bool testCollision)
{
	const Segment * segment = _segmentHandler->loadSegment(index);
	SegmentInstance* tempInstance = new SegmentInstance(segment, _endMatrix, true);
	// Check collision
	if (testCollision)
	{
		if(!bInsertIntoOctree(tempInstance))
        {
            delete tempInstance;
            return false;
        }
	}
	addWindowsAndZonesToSegment(segment);
	// Set up model matrix
	glm::mat4 modelEndMat = segment->getEndMatrix();
	int angle = static_cast<int>(360.f / _segmentHandler->getConnectionRotation(segment->getStart()));
	int maxRotOffset = segment->getInfo()->getRotationOffset(_curviness) / angle;
	float rotVal = 0.f;
	if (maxRotOffset != 0)
	{
		rotVal = static_cast<float>((rand() % (2 * maxRotOffset) - maxRotOffset) * angle);
	}
	glm::mat4 rotMat = glm::rotate(glm::radians(rotVal), glm::vec3(0, 0, 1));
	_endMatrix = _endMatrix * modelEndMat * rotMat;

	_generatedLength += segment->getLength();
	_track.push_back(tempInstance);
	return true;
}

// Insert the segment into the octree if it does not collide with
// anything in it.
// Return true if segment is inserted.
bool Track::bInsertIntoOctree(SegmentInstance* segment)
{
    CollisionMesh mesh(segment->getGlobalBoundingBoxes());
    assert(_octree->bTestCollision(mesh));

    return _octree->bInsertIf(mesh, segment, [this](const std::vector<SegmentInstance**>& collisions)
    {
        size_t numSegments = _track.size();
        size_t size = collisions.size();
        if(collisions.empty())
        {
            return true;
        }

        if(collisions.size() <= 2)
        {
            auto beginIt = _track.end() - collisions.size();
            for(SegmentInstance** const c : collisions)
            {
                if(std::find(beginIt, _track.end(), *c) == _track.end())
                {
                    return false;
                }
            }

            return true;
        }

        return false;
    });
}

bool Track::bTestCollision(const CollisionMesh& mesh) const
{
	if (_octree.get())
	{
		auto collisions = _octree->getCollisions(mesh);
		return collisions.size() != 0;
	}
	else
	{
		return false;
	}
}

// Inserts a whole pre-defined structure at the end of the track
void Track::insertStructure(const int index)
{
	float startLength = _generatedLength;
	const SegmentHandler::Structure * sugMig = _segmentHandler->getStructure(index);
	// Randomize how many times the structure should be looped
	int min = sugMig->minInRow;
	int max = sugMig->maxInRow;
	double scaled = (double)rand() / RAND_MAX;
	int amount = static_cast<int>((max - min + 1) * scaled) + min;
	// Randomize if there should be "negative" rotation
	int rotationDir = 1;
	if (rand() % 2 == 0)
	{
		rotationDir = -1;
	}
	// Amount of "loops"
	for (int i = 0; i < amount; i++)
	{
		// Amount of pieces in each "loop"
		for (unsigned int j = 0; j < sugMig->pieces.size(); j++)
		{
			const SegmentHandler::StructurePiece * p = sugMig->pieces[j];
			const Segment * segment = _segmentHandler->loadSegment(p->index);
			SegmentInstance* tempInstance = new SegmentInstance(segment, _endMatrix, true);

            if(!bInsertIntoOctree(tempInstance))
            {
                delete tempInstance;
                deleteSegments(_generatedLength - startLength + 300.f);
                _endMatrix = _track.back()->getModelMatrix() * _track.back()->getEndMatrix();
                return;
            }
			addWindowsAndZonesToSegment(segment);

			glm::mat4 modelEndMat = segment->getEndMatrix();
			int angle = static_cast<int>(360.f / _segmentHandler->getConnectionRotation(segment->getStart()));
			// Randomize angle from structure info
			int minRot = p->minRotation / angle;
			int maxRot = p->maxRotation / angle;
			scaled = (double)rand() / RAND_MAX;
			float rotVal = static_cast<float>(((maxRot - minRot) * scaled + minRot) * angle);
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

// Adds windows and zones to the track based on the given segment type
void Track::addWindowsAndZonesToSegment(const Segment* segment)
{
	// Add windows
	if (segment->bHasWindow())
	{
		_segmentWindows.push_back({ segment->getWindowModel(), _endMatrix, static_cast<unsigned int>(_track.size()) });
	}
	// Add temperature zones
	if (segment->nrOfZones() > 0)
	{
		std::vector<float> temperatures;
		temperatures.reserve(4);
		for (unsigned int i = 0; i < 4; i++)
		{
			int r = rand() % 8;
			if (r == 0)
			{
				temperatures.push_back((float)rand() / RAND_MAX - 1.f);
			}
			else if (r <= 2)
			{
				temperatures.push_back((float)rand() / RAND_MAX);
			}
			else
			{
				temperatures.push_back(-5.f);
			}
		}
		// Makes sure to only add something to render if at least one part of the zone is active
		unsigned int count = 0;
		for (unsigned int i = 0; i < 4; i++)
		{
			if (temperatures[i] > -2.f)
			{
				++count;
			}
		}
		if (count != 0)
		{
			_temperatureZones.push_back({ segment->getZonesModel(), _endMatrix, static_cast<unsigned int>(_track.size()), temperatures });
		}
	}
}

// Deletes a certain length of the track (from the end)
void Track::deleteSegments(const float lengthToDelete)
{
	float deletedLength = 0.f;
	while (deletedLength <= lengthToDelete && _generatedLength > 500.f)
	{
		int segmentLength = static_cast<int>(_track.back()->getLength());
		deletedLength += segmentLength;
		_generatedLength -= segmentLength;

		_octree->erase(_track.back());
		size_t index = _track.size() - 1;
		delete _track[index];
		_track.erase(_track.begin() + _track.size() - 1);
		// Windows
		if (_segmentWindows.size() >= 1 && _segmentWindows.back().segmentIndex == index)
		{
			_segmentWindows.pop_back();
		}
		// Temperature zones
		if (_temperatureZones.size() >= 1 && _temperatureZones.back().segmentIndex == index)
		{
			_temperatureZones.pop_back();
		}
	}
}

// Tries to end the track with a straight path
bool Track::bEndTrack()
{
	// Make sur the currently last segment is of end type a
	if (_endConnection != 'a')
	{
		for (unsigned int i = 1; i < _segmentHandler->infos().size(); i++)
		{
			const Segment * test = _segmentHandler->loadSegment(i);
			if (test->getStart() == _endConnection && test->getEnd() == 'a')
			{
				if (!bInsertNormalSegment(i, true))
				{
					deleteSegments(_endMargin + 500);
					_endMatrix = _track.back()->getModelMatrix() * _track.back()->getEndMatrix();
					return false;
				}
			}
		}
	}
	// Insert straight track
	while (_generatedLength < _targetLength)
	{
		if (!bInsertNormalSegment(0, true))
		{
			deleteSegments(_endMargin + 500);
			_endMatrix = _track.back()->getModelMatrix() * _track.back()->getEndMatrix();
			return false;
		}
	}
	return true;
}

// Places obstacles in the finished track
void Track::placeObstacles()
{
	const float endLength = 500.f;
	float currentLength = 300;
	float lastFullSegmentLength = 0.f;
	size_t index = findTrackIndex(currentLength, lastFullSegmentLength);
	ObstacleHandler::Obstacle * lastObstacleType = _obstacleHandler->getRandomObstacle(_difficulty);
	int inRow = 0;
	int currentInRow = 0;
	float leftOfArea = 0.f;
	const float invDiff = 1 - _difficulty;
	float backPadding = 0.f;
	while (currentLength < _generatedLength - endLength)
	{
		if (leftOfArea > 10.f)
		{
			ObstacleHandler::Obstacle * newObstacle = nullptr;
			if (currentInRow < inRow)
			{
				newObstacle = lastObstacleType;
				currentInRow++;
			}
			else
			{
				do
				{
					newObstacle = _obstacleHandler->getRandomObstacle(_difficulty);
				} while (newObstacle == lastObstacleType);
				backPadding = lastObstacleType->getBackPadding(_difficulty);
				lastObstacleType = newObstacle;
				inRow = rand() % (newObstacle->getMaxInRow(_difficulty) + 1);
				currentInRow = 1;
			}
			float lengthToNextObstacle = rand() % int(30 * invDiff + 10) + backPadding + newObstacle->getFrontPadding(_difficulty);
			currentLength += lengthToNextObstacle;
			index = findTrackIndex(currentLength, lastFullSegmentLength);

			std::vector<glm::vec3> waypoints = _track[index]->getParent()->getWaypoints();
			waypoints.push_back(glm::vec3(_track[index]->getEndMatrix() * glm::vec4(0, 0, 0, 1)));
			float targetDepth = currentLength - lastFullSegmentLength + 0.01f;

			// Calculate vectors between waypoints
			std::vector<glm::vec3> distanceVectors;
			distanceVectors.reserve(waypoints.size() - 1);
			for (size_t i = 0; i + 1 < waypoints.size(); i++)
			{
				distanceVectors.push_back(waypoints[i + 1] - waypoints[i]);
			}

			size_t distanceIndex = 0;
			float depth = 0.f;
			while (depth < targetDepth && distanceIndex < distanceVectors.size())
			{
				depth += glm::length(distanceVectors[distanceIndex]);
				distanceIndex++;
			}

			assert(distanceIndex > 0);
			distanceIndex--;
			const glm::vec3& finalDistance = distanceVectors[distanceIndex];
			float finalDistanceLength = glm::length(finalDistance);
			depth -= finalDistanceLength;
			float remainderDepth = targetDepth - depth;
			// Position
			glm::vec3 pos = waypoints[distanceIndex] + normalize(finalDistance) * remainderDepth;
			// Forward
			glm::vec3 v1 = glm::vec3(0, 0, 1);
			float factor = targetDepth / _track[index]->getLength();
			glm::vec3 v2 = _track[index]->getEndMatrix() * glm::vec4(0, 0, 1, 0);
			glm::vec3 forward = v1 * (1 - factor) + v2 * factor;

			glm::mat4 modelMat = _track[index]->getModelMatrix() * glm::inverse(glm::lookAt(pos, pos + forward, glm::vec3(0, 1, 0))) * glm::rotate(glm::radians(float(rand() % 360)), glm::vec3(0, 0, 1));
			_track[index]->addObstacle(ObstacleInstance(modelMat, newObstacle, _difficulty));

			leftOfArea -= lengthToNextObstacle;
		}
		else
		{
			currentLength += rand() % (int(500 * invDiff) + 50) + 70;
			leftOfArea = static_cast<float>(rand() % (int(600 * _difficulty) + 50) + 100);
		}
	}
}

// Place dark areas in the finished track
void Track::placeDarkAreas()
{
	int index = rand() % 400 + 20;
	while (index < _track.size() - 10)
	{
		unsigned int length = rand() % 10 + 7;
		_darkAreas.push_back({ (unsigned int)index, length, 0.f });
		index += length;
		index += rand() % 200 + 100;
	}
	// Removes the last area if it is too long
	if (_darkAreas.size() >= 1 && _darkAreas.back().startIndex + _darkAreas.back().length > _track.size() - 10)
	{
		_darkAreas.erase(_darkAreas.end());
	}
}

size_t Track::findTrackIndex(const float totalLength, float & lastFullSegmentLength) const
{
	float traversedLength = 0;
	size_t index = 0;
	while (traversedLength <= totalLength)
	{
		float l = _track[index]->getLength();
		traversedLength += l;
		index++;
	}
	index--;
	lastFullSegmentLength = traversedLength - _track[index]->getLength();
	return index;
}

// Update stuff
void Track::update(const float dt, const std::vector<unsigned int> playerIndexes)
{
	// Calculate which player index is the smallest and largest
	unsigned int firstPlayer = 0;
	unsigned int lastPlayer = (unsigned int)_track.size() - 1;
	for (unsigned int i = 0; i < playerIndexes.size(); i++)
	{
		unsigned int index = playerIndexes[i];
		if (index >= firstPlayer)
		{
			firstPlayer = index;
		}
		if (index <= lastPlayer)
		{
			lastPlayer = index;
		}
	}
	// Obstacles
	for (unsigned int i = lastPlayer; i < _track.size() && i < firstPlayer + 7; ++i)
	{
		_track[i]->update(dt);
	}
	for (unsigned int i = lastPlayer; i <= firstPlayer; ++i)
	{
		_track[i]->decreaseSpeed(dt);
	}
	// Temperature zones
	for (unsigned int i = 0; i < _temperatureZones.size(); ++i)
	{
		if (_temperatureZones[i].segmentIndex >= lastPlayer)
		{
			if (_temperatureZones[i].segmentIndex <= firstPlayer)
			{
				std::vector<float>& temps = _temperatureZones[i].temperatures;
				for (unsigned int j = 0; j < temps.size(); ++j)
				{
					if (temps[j] > -0.99f)
					{
						temps[j] -= 0.1f * dt;
					}
				}

				_track[_temperatureZones[i].segmentIndex]->setTemperatures(temps);
			}
			else
			{
				break;
			}
		}
	}

	// Update dark areas
	for (unsigned int i = 0; i < _darkAreas.size(); i++)
	{
		if (_darkAreas[i].startIndex >= lastPlayer)
		{
			if (_darkAreas[i].startIndex + _darkAreas[i].length <= firstPlayer)
			{
				_darkAreas[i].timer += dt;
				if (_darkAreas[i].timer > 1.0f && _darkAreas[i].length > 1)
				{
					_darkAreas[i].timer = 0.f;
					_darkAreas[i].length--;
				}
			}
			else
			{
				break;
			}
		}
	}
	// Update player dark area info
	for (unsigned int i = 0; i < playerIndexes.size(); i++)
	{
		DarkAreaPlayerInfo& dapi = _darkAreaPlayerValues[i];
		// Not currently inside a dark area
		if (dapi.startIndex == -1)
		{
			// Update darkness factor for the player
			dapi.factor -= 1.0f * dt;
			if (dapi.factor < 0.f)
			{
				dapi.factor = 0.f;
			}
			// Check if inside dark area
			for (unsigned int j = 0; j < _darkAreas.size(); j++)
			{
				if (playerIndexes[i] >= _darkAreas[j].startIndex && playerIndexes[i] <= _darkAreas[j].startIndex + dapi.length)
				{
					dapi.startIndex = _darkAreas[j].startIndex;
					dapi.length = _darkAreas[j].length;
				}
			}
		}
		// Currently inside dark area
		else
		{
			// Update values if inside area
			dapi.factor += 1.0f * dt;
			if (dapi.factor > 0.96f)
			{
				dapi.factor = 0.96f;
			}
			if (playerIndexes[i] > dapi.startIndex + dapi.length)
			{
				dapi.startIndex = -1;
			}
		}
	}
}

// Render the track
void Track::render(GFX::ViewportPipeline& pipeline, const unsigned int playerIndex, const unsigned int shipIndex)
{
	// Inside of segments
	for (int i = -2; i < 7; ++i)
	{
		int index = shipIndex + i;
		if (index >= 0 && index < _track.size())
		{
			pipeline.generalForward.render(*_track[index]);
			std::vector<ObstacleInstance>& obstacles = _track[index]->getObstacles();
			for (auto& obstacle : obstacles)
			{
				pipeline.transparentForward.render(obstacle);
			}
		}
	}

	// Outside of segments
	for (unsigned int i = 1; i < 100; ++i)
	{
		size_t index = shipIndex + i;
		if (index >= 0 && index < _track.size())
		{
			pipeline.windowForward.render(*_track[index]);
		}
	}


	int lowestIndex = shipIndex - 2;
	int largestIndex = shipIndex + 7;
	// Segment windows
	for (auto& window : _segmentWindows)
	{
		int windowIndex = static_cast<int>(window.segmentIndex);
		if (windowIndex >= lowestIndex )
		{
			if (windowIndex < largestIndex)
			{
				pipeline.windowForward.render(window);
			}
			else
			{
				break;
			}
		}
	}
	// Temperature zones
	for (auto& zone : _temperatureZones)
	{
		int zoneIndex = static_cast<int>(zone.segmentIndex);
		if (zoneIndex >= lowestIndex)
		{
			if (zoneIndex < largestIndex)
			{
				pipeline.zoneForward.render(zone);
			}
			else
			{
				break;
			}
		}
	}

	// Dark areas
	pipeline.setDarkFactor(_darkAreaPlayerValues[playerIndex].factor);
}

WaypointInfo Track::findNextWaypointInfo(const WaypointInfo& current, unsigned int segmentIndex) const
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
