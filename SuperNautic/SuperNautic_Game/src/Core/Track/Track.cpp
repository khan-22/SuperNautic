#include <glm\gtx\transform.hpp>
#include "glm/gtx/norm.hpp"
#include <time.h>

#include "Track.hpp"
#include "../../Log.hpp"
#include "SegmentInfo.hpp"
#include "../../GFX/Model.hpp"
#include "Segment.hpp"

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
	if (length >= 3000 && length <= 100000)
	{
		_targetLength = length;
		return true;
	}
	LOG_ERROR("Track not long enough!!!");
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
		insertNormalSegment(0, totalLength, false);
	}
	char end = 'a';
	bool lighting = true;
	int prevIndex = -1;
	// Create random path
	while (totalLength < _targetLength - 500 && end == 'a')
	{
		// Randomize segment index
		int index;
		int inRow;
		do
		{
			index = getIndex(end);
			if (index == -1)
			{
				LOG_ERROR("WARNING! Something went wrong with the track generation! Not enough connections of type '", end, "'.");
			}
		} while (index == prevIndex);
		// Randomize nr of same segment type in row
		inRow = getInRow(index);
		// Instanciate the segment(s)
		bool collided = false;
		for (unsigned int i = 0; i < inRow; i++)
		{
			insertNormalSegment(0, totalLength, false);
			//insertStructure(0, totalLength);
			/*if (!insertNormalSegment(index, totalLength, true))
			{
				deleteSegments(totalLength, 200);
				_endMatrix = _track.back()->getModelMatrix() * _track[_track.size() - 1]->getEndMatrix();
				collided = true;
				break;
			}*/
		}
		prevIndex = _track[_track.size() - 1]->getIndex();
	}
	// Make the final stretch straight
	while (totalLength < _targetLength)
	{
		insertNormalSegment(0, totalLength, false);
	}
	_generatedLength = totalLength;

	LOG("Track generated. Length: " + std::to_string(_generatedLength) + " meters.");
	generateObstacles();
	return true;
}

void Track::generateObstacles()
{
    for(SegmentInstance* segment : _track)
    {
        const std::vector<glm::vec3>& waypoints = segment->getParent()->getWaypoints();
        assert(waypoints.size() > 0);
        float targetDepth = rand() % (segment->getLength() - 1) + 1;

        std::vector<glm::vec3> distanceVectors;
        distanceVectors.reserve(waypoints.size() - 1);
        for(size_t i = 0; i + 1 < waypoints.size(); i++)
        {
            distanceVectors.push_back(waypoints[i + 1] - waypoints[i]);
        }

        size_t distanceIndex = 0;
        float depth = 0.f;
        while(depth < targetDepth && distanceIndex < distanceVectors.size())
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

        glm::vec3 rayStart = waypoints[distanceIndex] + finalDistance * (remainderDepth / finalDistanceLength);

        glm::vec3 planeNormal = finalDistance / finalDistanceLength;

        // ax + by + bz + d = 0
        float d = -glm::dot(planeNormal, rayStart);

        // Generate random point on plane.
        float xModifier = std::fabs(planeNormal.x) > 0.001f ? 1.f : 0.f;
        float yModifier = std::fabs(planeNormal.y) > 0.001f ? 1.f : 0.f;
        glm::vec3 planePoint;
        do
        {
            planePoint.x = float(rand() % 1000 - 500) * xModifier;
            planePoint.y = float(rand() % 1000 - 500) * yModifier;
            planePoint.z = -(d + planeNormal.x * planePoint.x + planeNormal.y * planePoint.y) / planeNormal.z;
        } while(glm::distance2(planePoint, rayStart) > 0.01f);

        glm::vec3 rayDirection = glm::normalize(planePoint - rayStart);

        Ray ray(rayStart, rayDirection, 100.f);
        RayIntersection intersection = segment->getParent()->rayIntersectionTest(ray);

        assert(intersection._hit == true); // May happen... In that case you could regenerate the ray until it intersects.


        BoundingBox box;
        box.center = segment->getModelMatrix() * glm::vec4(intersection._position, 1.f);
        box.directions =
        {
            glm::vec3(1.f, 0.f, 0.f),
            glm::vec3(0.f, 1.f, 0.f),
            glm::vec3(0.f, 0.f, 1.f)
        };
        box.halfLengths = {1.f, 1.f, 1.f};

        _obstacles.emplace_back(box);
    }
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

	for(GFX::Box& obstacle : _obstacles)
    {
        renderer.render(obstacle);
    }
}

// Inserts a segment with given index at the end of the track
bool Track::insertNormalSegment(const int index, int & length, bool testCollision)
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

void Track::insertStructure(const int index, int & length)
{
	const SegmentHandler::Structure * s = _segmentHandler->getStructure(index);
	int min = s->minInRow;
	int max = s->maxInRow;
	double scaled = (double)rand() / RAND_MAX;
	int amount = (max - min + 1) * scaled + min;
	for (unsigned int i = 0; i < amount; i++)
	{
		for (unsigned int j = 0; j < s->pieces.size(); j++)
		{
			const SegmentHandler::StructurePiece * p = s->pieces[j];
			const Segment * segment = _segmentHandler->loadSegment(p->index);
			SegmentInstance* tempInstance = new SegmentInstance(segment, _endMatrix, true);
			glm::mat4 modelEndMat = segment->getEndMatrix();
			int angle = 360.f / _segmentHandler->getConnectionRotation(segment->getStart());

			int minRot = p->minRotation / angle;
			int maxRot = p->maxRotation / angle;
			double scaled = (double)rand() / RAND_MAX;
			float rotVal = ((maxRot - minRot) * scaled + minRot) * angle;

			glm::mat4 rotMat = glm::rotate(glm::radians(rotVal), glm::vec3(0, 0, 1));
			_endMatrix = _endMatrix * modelEndMat * rotMat;
			length += segment->getLength();
			_track.push_back(tempInstance);
		}
	}
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
