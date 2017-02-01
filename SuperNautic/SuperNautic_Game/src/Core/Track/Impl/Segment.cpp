#include <unordered_map>
#include <algorithm>
#include <limits>

#include "../Segment.hpp"
#include "../../../Log.hpp"

// Static constants declared in Segment.hpp
const std::string Segment::baseVisualName					{ "VM" };
const std::string Segment::baseCollisionName				{ "CM" };
const std::string Segment::temperatureZoneVisualName		{ "zoneVisual" };
const std::string Segment::temperatureZoneCollisionName		{ "zoneCollision" };
const std::string Segment::boundingBoxName					{ "BB" };
const std::string Segment::waypointsName					{ "WP" };
const std::string Segment::endName							{ "end" };

// Static maps declared in Segment.hpp
std::unordered_map<std::string, unsigned Segment::*> Segment::nameToIndex;
std::unordered_map<std::string, std::vector<unsigned> Segment::*> Segment::nameToVecIndex;

// Mappings are by default not created
bool Segment::mappingsCreated { false };


// Loads a segment from an fbx file
Segment::Segment(std::string dataFilePath, std::string visualFilePath, std::string startConnection, std::string endConnection)
	: _segmentName { dataFilePath }, _startConnection { startConnection }, _endConnection { endConnection }
{
	// Get the scene data asset
	_scene = RawMeshCache::get(std::string{ "Segments\\" } + dataFilePath);

	// Get visual model asset 
	_visual = ModelCache::get(std::string{ "Segments\\" } + visualFilePath);

	if (_scene.get()->cameras.size() < 1)
	{
		LOG_ERROR("No camera found in scene. A camera representing the end point is required");
		return;
	}

	// Create mappings, if required
	if (!mappingsCreated)
	{
		initializeMappings();
	}

	// Load mesh pointers into appropriate member variables
	if (_scene.get()->meshes.size() > 0)
	{
		assignMeshPointers();
	}
	else
	{
		LOG_ERROR("No meshes in file ", dataFilePath);
		return;
	}

	// Create bounding boxes from meshes
	createBoundingBoxes();

	// Create waypoints from meshes
	createWaypoints();
}

// Tests a ray collision against all collision surfaces of the segment. Returns collision information
const Intersection Segment::rayIntersectionTest(glm::vec3 origin, glm::vec3 direction) const
{
	// incomplete
	return Intersection{};
}

// Finds the two waypoints closest to a position (position is relative to segment's local origin)
std::pair<glm::vec3, glm::vec3> Segment::findClosestWaypoints(const glm::vec3& position) const
{
	// Will hold the closest two vectors, initialize to first two waypoints
	// First is closer than second
	std::pair<glm::vec3, glm::vec3> closest { glm::vec3{_waypoints[0]}, glm::vec3{_waypoints[1]} };

	float firstDistance = squaredDistance(closest.first, position);
	float secondDistance = squaredDistance(closest.second, position);

	// Swap if second is closer than first
	if (secondDistance < firstDistance)
	{
		std::swap(closest.first, closest.second);
		std::swap(firstDistance, secondDistance);
	}

	// Check rest of waypoints
	for (unsigned i = 2; i < _waypoints.size(); ++i)
	{
		float testDistance = squaredDistance(_waypoints[i], position);

		if (testDistance < secondDistance)
		{
			// This is the closest waypoint so far, first becomes second, _waypoints[i] becomes first
			if (testDistance < firstDistance)
			{
				// Set first to second
				closest.second = closest.first;
				secondDistance = firstDistance;

				// Set test to first
				closest.first = _waypoints[i];
				firstDistance = testDistance;
			}
			// Second closest so far
			else
			{
				// Set test to second
				closest.second = _waypoints[i];
				secondDistance = testDistance;
			}
		}
	}

	return closest;
}

// Renders the segment at the position of an instance
// TODO

// Initializes nameToIndex and nameToVecIndex
void Segment::initializeMappings()
{
	nameToIndex[baseVisualName] =		&Segment::_baseVisual;
	nameToIndex[baseCollisionName] =	&Segment::_baseCollision;

	nameToVecIndex[waypointsName] =					&Segment::_waypointMeshes;
	nameToVecIndex[temperatureZoneVisualName] =		&Segment::_temperatureZoneVisuals;
	nameToVecIndex[temperatureZoneCollisionName] =	&Segment::_temperatureZoneCollisions;
	nameToVecIndex[boundingBoxName] =				&Segment::_boundingBoxMeshes;

	mappingsCreated = true;
}

// Iterate through meshes in scene and assign to proper pointers
void Segment::assignMeshPointers()
{
	std::string currentName;
	for (unsigned i = 0; i < _scene.get()->meshes.size(); ++i)
	{
		// currentName contains name of current mesh
		// readName will not have trailing digits removed, used for error logging
		std::string readName = currentName = _scene.get()->meshes[i].name;

		// Will hold the number at the end of the mesh name
		std::string trailingNumberString;
		int trailingNumber { -1 };

		// Remove any numbers at end of name
		while (isdigit(currentName[currentName.length() - 1]))
		{
			trailingNumberString.insert(trailingNumberString.begin(), currentName[currentName.length() - 1]);
			currentName.pop_back();
		}

		if (trailingNumberString.length() > 0)
		{
			trailingNumber = atoi(trailingNumberString.data());
		}

		// If currentName is equal to a name mapped to a mesh index, 
		// set the current mesh to appropriate class member
		if (nameToIndex.find(currentName) != nameToIndex.end())
		{
			this->*nameToIndex[currentName] = i;
		}
		// If currentName is equal to a name mapped to a vector<aiMesh*>, 
		// add the current mesh to the appropriate class member vector
		else if (nameToVecIndex.find(currentName) != nameToVecIndex.end())
		{
			// If mesh is numbered, place meshes in order
			if (trailingNumber >= 0)
			{
				// Resize to fit
				if ((this->*nameToVecIndex[currentName]).size() < (trailingNumber + 1))
				{
																					// Max value represents unassigned
					(this->*nameToVecIndex[currentName]).resize(trailingNumber + 1, std::numeric_limits<unsigned>::max());
				}
				(this->*nameToVecIndex[currentName])[trailingNumber] = i;
			}
			// Else, place in read order
			else
			{
				(this->*nameToVecIndex[currentName]).push_back(i);
			}
		}
		else
		{
			// Mesh name not recognized
			LOG_ERROR("Mesh name not recognized: ", readName);
		}
	}

	if (_temperatureZoneCollisions.size() != _temperatureZoneVisuals.size())
	{
		LOG_ERROR("Mismatch: number of visual temperature models not equal to number of collision models in segment ", _segmentName);
	}
}

// Create bounding boxes from _boundingBoxMeshes
void Segment::createBoundingBoxes()
{
	// Create a bounding box for every mesh in _boundingBoxMeshes
	for (unsigned i = 0; i < _boundingBoxMeshes.size(); ++i)
	{
		// Max value represents unassigned
		if (_boundingBoxMeshes[i] == std::numeric_limits<unsigned>::max())
		{
			LOG_ERROR("_boundingBoxMeshes[", i, "] was not assigned (not zero-based indexing?)");
			continue;
		}

		// Check for correct number of faces
		if (_scene.get()->meshes[_boundingBoxMeshes[i]].faces.size() != 12)
		{
			LOG_ERROR("A bounding box mesh was not a box");
			continue;
		}
		
		BoundingBox box {};

		// Get middle of box
		box.center = findMeshCenter(_boundingBoxMeshes[i]);

		// Find first two directions and half-lengths of box
		findTwoDirections(_boundingBoxMeshes[i], box);

		box.halfLengths[2] = findThirdHalfDistance(_boundingBoxMeshes[i], box);
		if (box.halfLengths[2] <= 0.0f)
		{
			LOG_ERROR("Could not find the third half-length of a bounding box");
		}
		else
		{
			// Box created successfully
			_boundingBoxes.push_back(box);
		}
	}
}

// Create waypoints from _waypointMeshes
void Segment::createWaypoints()
{
	// Create vector of average positions, middle[0] is always (0, 0, 0)
	createAverageWaypoints(_waypointMeshes);

	if (_waypoints.size() < 2)
	{
		LOG_ERROR("Segment ", _segmentName, " has fewer than 2 waypoints. Not good :(");
		return;
	}

	// Will accumulate the length between waypoints
	float length = 0.0f;

	// Set middle[i] to the waypoint closest to middle[i - 1]
	// middle[0] is always (0, 0, 0)
	for (unsigned i = 1; i < _waypoints.size(); ++i)
	{
		// Index of closest, initialize to first position tested
		unsigned closest = i;

		// Squared distance from middles[i] to middles[i - 1], will hold distance from closest waypoint to middles[i - 1]
		float lowestDistance = squaredDistance(_waypoints[i], _waypoints[i - 1]);

		for (unsigned j = i + 1; j < _waypoints.size(); ++j)
		{
			float testDistance = squaredDistance(_waypoints[j], _waypoints[i]);

			if (testDistance < lowestDistance)
			{
				lowestDistance = testDistance;
				closest = j;
			}
		}

		// Move closest point to position 0 in vector
		if (closest != i)
		{
			std::swap(_waypoints[i], _waypoints[closest]);
		}

		length += sqrtf(lowestDistance);
	}

	// Initialize _length
	_length = length;
}

// Create a vector with average positions of a vector of mesh indices, insert (0, 0, 0) at index 0
void Segment::createAverageWaypoints(std::vector<unsigned>& meshIndices)
{
	// Create a vector containing the average position of all vertices in corresponding mesh in meshes
	// First waypoint is always (0, 0, 0)
	_waypoints.push_back( glm::vec3{0, 0, 0} );

	for (unsigned i = 0; i < meshIndices.size(); ++i)
	{
		if (meshIndices[i] == std::numeric_limits<unsigned>::max())
		{
			LOG_ERROR("_waypointMeshes[", i, "] was not assigned (not zero-based indexing?)");
			continue;
		}

		// Initialize average position
		_waypoints.push_back(glm::vec3{ 0, 0, 0 });
		const size_t index = _waypoints.size() - 1;

		// Add the position of every vertex in the model
		for (unsigned j = 0; j < _scene.get()->meshes[meshIndices[i]].vertices.size(); ++j)
		{
			_waypoints[index].x += _scene.get()->meshes[meshIndices[i]].vertices[j].x;
			_waypoints[index].y += _scene.get()->meshes[meshIndices[i]].vertices[j].y;
			_waypoints[index].z += _scene.get()->meshes[meshIndices[i]].vertices[j].z;
		}

		// Divide by number of vertices to obtain average
		_waypoints[index] /= _scene.get()->meshes[meshIndices[i]].vertices.size();
	}
}

// Find the center of mesh _scene->meshes[meshIndex]
glm::vec3 Segment::findMeshCenter(unsigned meshIndex) const
{
	glm::vec3 center {0, 0, 0};

	// Add position of every vertex
	for (unsigned i = 0; i < _scene.get()->meshes[meshIndex].vertices.size(); ++i)
	{
		center.x += _scene.get()->meshes[meshIndex].vertices[i].x;
		center.y += _scene.get()->meshes[meshIndex].vertices[i].y;
		center.z += _scene.get()->meshes[meshIndex].vertices[i].z;
	}

	// Divide by number of vertices and return
	return (center /= _scene.get()->meshes[meshIndex].vertices.size());
}

// Helper function for createBoundingBoxes(). Finds the third half-distance of a box
float Segment::findThirdHalfDistance(unsigned boundingBoxMeshIndex, const BoundingBox& box) const
{
	// Reduce initializer length
	std::vector<glm::vec3>& vertices = _scene.get()->meshes[boundingBoxMeshIndex].vertices;
	std::vector<glm::uvec3>& faces = _scene.get()->meshes[boundingBoxMeshIndex].faces;

	// Loop through faces to find an edge along the third direction. This will be used to find the third half-length
	for (unsigned j = 1; j < faces.size(); ++j)
	{
		// Construct vertices for this face
		std::array<glm::vec3, 3> faceVertices{
			glm::vec3{ vertices[faces[j].x] },
			glm::vec3{ vertices[faces[j].y] },
			glm::vec3{ vertices[faces[j].z] }
		};

		// Construct vectors between vertices of this face
		std::array<glm::vec3, 3> faceVectors{ glm::vec3{ faceVertices[0] - faceVertices[1] },
			glm::vec3{ faceVertices[0] - faceVertices[2] },
			glm::vec3{ faceVertices[1] - faceVertices[2] }
		};

		// If a vector points in the same or opposite direction as the third box direction, the distance can be found from its length
		for (unsigned k = 0; k < 3; ++k)
		{
			if (bAlmostEqual(glm::normalize(faceVectors[k]), box.directions[2]) ||
				bAlmostEqual(glm::normalize(-faceVectors[k]), box.directions[2]))
			{
				return glm::length(faceVectors[k]) / 2.0f;
			}
		}
	}

	return -1.0f;
}

// Helper function for createBoundingBoxes(). Finds three directions and two half-lengths of a box using the first face of the box mesh
void Segment::findTwoDirections(unsigned boundingBoxMeshIndex, BoundingBox& box) const
{
	// Reduce initializer length
	std::vector<glm::vec3>& vertices = _scene.get()->meshes[boundingBoxMeshIndex].vertices;
	std::vector<glm::uvec3>& faces = _scene.get()->meshes[boundingBoxMeshIndex].faces;

	// Vertices of first face in bounding box, will be used to construct two direction vectors
	std::array<glm::vec3, 3> firstFaceVertices{
		glm::vec3{ vertices[faces[0].x] },
		glm::vec3{ vertices[faces[0].y] },
		glm::vec3{ vertices[faces[0].z] }
	};

	// Initialize three vectors, only two will be kept (the longest is not part of a box edge)
	box.directions[0] = firstFaceVertices[0] - firstFaceVertices[1];
	float length0 = glm::length(box.directions[0]);
	box.directions[1] = firstFaceVertices[0] - firstFaceVertices[2];
	float length1 = glm::length(box.directions[1]);
	box.directions[2] = firstFaceVertices[1] - firstFaceVertices[2];
	float length2 = glm::length(box.directions[2]);

	// Swap vectors so that box.directions[2] is the longest, as it will be replaced
	if (length2 < length0)
	{
		std::swap(box.directions[2], box.directions[0]);
		std::swap(length2, length0);
	}
	if (length2 < length1)
	{
		std::swap(box.directions[2], box.directions[1]);
		std::swap(length2, length1);
	}

	// Normalize first two vectors
	box.directions[0] = glm::normalize(box.directions[0]);
	box.directions[1] = glm::normalize(box.directions[1]);

	// The third vector is the cross product of the first two
	box.directions[2] = glm::cross(box.directions[0], box.directions[1]);

	// length0 and length1 are halved to find halfLengths of x and y
	box.halfLengths[0] = length0 / 2.0f;
	box.halfLengths[1] = length1 / 2.0f;
}