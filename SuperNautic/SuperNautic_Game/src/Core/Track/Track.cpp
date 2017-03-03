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
	, _targetLength(10000)
	, _generatedLength(0)
	, _totalProgress(0.f)
	, _endMargin(400)
	, _endMatrix(glm::mat4())
{
	//_octrees.push_back(Octree<SegmentInstance*>(glm::vec3(0, 0, 0), 10000));
	_progressionLength = 1500.f;
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

unsigned int Track::getCurviness() const
{
    return _MAX_CURVINESS * _curviness;
}

unsigned int Track::getDifficulty() const
{
    return _MAX_DIFFICULTY * _difficulty;
}


// Sets the track length in whole meters
void Track::setLength(const unsigned int length)
{
	assert(length >= _MIN_LENGTH && length <= _MAX_LENGTH);
	_targetLength = length;
}

// Set randomization seed
void Track::setSeed(const std::string& seed)
{
	_seed = seed;
	if (seed.length() == 1 && seed[0] == '1')
	{
		srand(static_cast<unsigned>(time(NULL)));
	}
	else
	{
		unsigned int intSeed = 0;
		for (unsigned int i = 0; i < seed.length(); i++)
		{
			intSeed += (unsigned int)((seed[i] - 65) * powf(10, i));
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
	_octrees.clear();
	for (unsigned int i = 0; i < _track.size(); i++)
	{
		delete _track[i];
	}
	_track.clear();
}

// Generates the track
bool Track::bGenerate()
{
	// Make the inital stretch straight
	while (_generatedLength < 300)
	{
		bInsertNormalSegment(0, false);
	}

	int failedRecently = 0;
	// Create random path
	while (_generatedLength - _lengthAfterLastCall < _progressionLength)
	{
		// Randomize segment index
		int index, inRow;
		index = getIndex();
		// Normal segment placement
		if (index < _segmentHandler->infos().size())
		{
			// Randomize nr of same segment type in a row
			inRow = getInRow(index);

			for (int i = 0; i < inRow; i++)
			{
				if (!bInsertNormalSegment(index, true))
				{
					if (failedRecently != 0)
					{
						failedRecently += 400;
					}
					failedRecently += 500;
					deleteSegments(failedRecently);
					_endMatrix = _track.back()->getModelMatrix() * _track.back()->getEndMatrix();
					break;
				}
				else
				{
					if (failedRecently > 0)
					{
						failedRecently -= _lastSegment->getLength();
						if (failedRecently < 0)
						{
							failedRecently = 0;
						}
					}
				}
			}
		}
		// Structures
		else
		{
			insertStructure(index - static_cast<int>(_segmentHandler->infos().size()));
		}
		_endConnection = _track.back()->getParent()->getEnd();
		_prevIndex = index;
		_lastSegment = _track.back()->getParent();

		// Make the final stretch straight
		if (_generatedLength > _targetLength - _endMargin)
		{
			if (bEndTrack())
			{
				if (_difficulty > 0.05)
				{
					placeObstacles();
				}
				LOG("Track generated. Length: ", _generatedLength);
				return true;
			}
			else
			{
				_endMatrix = _track.back()->getModelMatrix() * _track.back()->getEndMatrix();
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
		for (int i = 0; i < _segmentHandler->getNrOfStructures(); i++)
		{
			if (infos[_segmentHandler->getStructure(i)->pieces[0]->index]._startConnection == _endConnection
				&& i != _prevIndex && _segmentHandler->getStructure(i)->curviness <= _curviness)
			{
				validSegments.push_back(static_cast<int>(infos.size()) + i);
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
				int index = validSegments[i] - static_cast<int>(infos.size());
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
				int test = _segmentHandler->getStructure(validSegments[i] - static_cast<int>(infos.size()))->getProbability(_curviness);
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


glm::vec3 Track::findForward(const glm::vec3 globalPosition, unsigned& segmentIndex, glm::vec3& returnPos, float& lengthInSegment, glm::vec3& directionDifference)
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
	if (distanceToPlane <= 0.0f)	// Protect against the case where the track runs out of waypoints ahead of the player
	{
		interpolationValue = 1.0f;
	}

	// Calculate current length in segment
	lengthInSegment = 0.0f;

	// Add previous waypoint lengths
	for (unsigned i = 0; i < closest.index; ++i)
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
	if (testCollision)
	{
		//						const std::function<bool(const std::vector<ElementT*>&)>& predicate
		/*auto predicate = [this](const std::function<bool(const std::vector<SegmentInstance*>&)>& segments)
		{

		};
		for (size_t i = 0; i < _octrees.size(); i++)
		{
			if (_octrees[i].bInsertIf(tempInstance->getGlobalBoundingBoxes()[0], tempInstance, predicate))
			{

			}
		}*/

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
	int angle = static_cast<int>(360.f / _segmentHandler->getConnectionRotation(segment->getStart()));
	int maxRotOffset = segment->getInfo()->getRotationOffset(_curviness) / angle;
	if(maxRotOffset == 0)
    {
        maxRotOffset = 1;
    }
	int rotVal = (rand() % (2 * maxRotOffset) - maxRotOffset) * angle;
	glm::mat4 rotMat = glm::rotate(glm::radians(static_cast<float>(rotVal)), glm::vec3(0, 0, 1));
	_endMatrix = _endMatrix * modelEndMat * rotMat;
	_generatedLength += static_cast<int>(segment->getLength());
	if (segment->bHasWindow())
	{
		_segmentWindows.push_back({ segment->getWindowModel(), static_cast<unsigned int>(_track.size()), tempInstance->getModelMatrix() });
	}
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
					deleteSegments(_generatedLength - startLength + 300);
					_endMatrix = _track.back()->getModelMatrix() * _track.back()->getEndMatrix();
					return;
				}
			}
			glm::mat4 modelEndMat = segment->getEndMatrix();
			int angle = static_cast<int>(360.f / _segmentHandler->getConnectionRotation(segment->getStart()));
			// Randomize angle from structure info
			int minRot = p->minRotation / angle;
			int maxRot = p->maxRotation / angle;
			double scaled = (double)rand() / RAND_MAX;
			float rotVal = static_cast<float>(((maxRot - minRot) * scaled + minRot) * angle);
			// Finalizing
			glm::mat4 rotMat = glm::rotate(glm::radians(rotVal * rotationDir), glm::vec3(0, 0, 1));
			_endMatrix = _endMatrix * modelEndMat * rotMat;
			_generatedLength += static_cast<int>(segment->getLength());
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
		if (_endConnection == 'a' && !bInsertNormalSegment(0, true))
		{
			deleteSegments(_endMargin + 400);
			_endMatrix = _track.back()->getModelMatrix() * _track.back()->getEndMatrix();
			return false;
		}
		if (_endConnection == 'b' && !bInsertNormalSegment(1, true))
		{
			deleteSegments(_endMargin + 400);
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
			leftOfArea = rand() % (int(600 * _difficulty) + 50) + 100;
		}
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

// Update obstacle rotations
void Track::update(const float dt, const unsigned int firstPlayer, const unsigned int lastPlayer)
{
	_track[firstPlayer]->decreaseObstacleSpeed();
	for (size_t i = lastPlayer; i < _track.size() && i < firstPlayer + 6; i++)
	{
		_track[i]->update(dt);
	}
}

// Render the track
void Track::render(GFX::DeferredRenderer& renderer, GFX::WindowRenderer& windowRenderer, const int shipIndex)
{
	for (int i = -2; i < 7; i++)
	{
		int index = shipIndex + i;
		if (index >= 0 && index < _track.size())
		{
			renderer.render(*_track[index]);
			std::vector<ObstacleInstance>& obstacles = _track[index]->getObstacles();
			for (auto& obstacle : obstacles)
			{
				renderer.render(obstacle);
			}
		}
	}

	for (int i = 1; i < 100; i++)
	{
		int index = shipIndex + i;
		if (index >= 0 && index < _track.size())
		{
			windowRenderer.render(*_track[index]);
		}
	}


	int lowestIndex = shipIndex - 2;
	int largestIndex = shipIndex + 7;

	for (auto& window : _segmentWindows)
	{
		int windowIndex = static_cast<int>(window.segmentIndex);
		if (windowIndex >= lowestIndex )
		{
			if (windowIndex < largestIndex)
			{
				windowRenderer.render(window);
			}
			else
			{
				break;
			}
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
