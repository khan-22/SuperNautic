#include <unordered_map>
#include <algorithm>

#include "../Segment.hpp"
#include "../../../Log.hpp"


// Static constants declared in Segment.hpp
const std::string Segment::baseVisualName					{ "baseVisual" };
const std::string Segment::baseCollisionName				{ "baseCollision" };
const std::string Segment::temperatureZoneVisualName		{ "zoneVisual" };
const std::string Segment::temperatureZoneCollisionName		{ "zoneCollision" };
const std::string Segment::boundingBoxName					{ "boundingBox" };
const std::string Segment::waypointsName					{ "waypoint" };
const std::string Segment::endName							{ "end" };

// Static maps declared in Segment.hpp
std::unordered_map<std::string, aiMesh* Segment::*> Segment::nameToPtr;
std::unordered_map<std::string, std::vector<aiMesh*> Segment::*> Segment::nameToVecPtr;

// Mappings are by default not created
bool Segment::mappingsCreated { false };


// Loads a segment from an fbx file
Segment::Segment(std::string path, std::string fileName, std::string startConnection, std::string endConnection)
	: _segmentName {path + fileName}, _startConnection {startConnection}, _endConnection {endConnection}
{
	// Load the scene from an fbx file
	_scene = _importer.ReadFile(fileName.c_str(), 0);

	if (_scene->mNumCameras < 1)
	{
		LOG_ERROR("No camera found in scene. A camera representing the end point is required");
		return;
	}

	// Construct end matrix
	aiMatrix4x4 aiEndMatrix; 
	_scene->mCameras[0]->GetCameraMatrix(aiEndMatrix);

	_endMatrix = { aiEndMatrix.a1, aiEndMatrix.a2, aiEndMatrix.a3, aiEndMatrix.a4,
				   aiEndMatrix.b1, aiEndMatrix.b2, aiEndMatrix.b3, aiEndMatrix.b4, 
				   aiEndMatrix.c1, aiEndMatrix.c2, aiEndMatrix.c3, aiEndMatrix.c4,
				   aiEndMatrix.d1, aiEndMatrix.d2, aiEndMatrix.d3, aiEndMatrix.d4 };

	// Create mappings, if required
	if (!mappingsCreated)
	{
		initializeMappings();
	}

	// Load mesh pointers into appropriate member variables
	if (_scene->mNumMeshes > 0)
	{
		assignMeshPointers();
	}
	else
	{
		LOG_ERROR("No meshes in file ", fileName);
		return;
	}

	// Create bounding boxes from meshes
	_boundingBoxes = createBoundingBoxes();

	// Create waypoints from meshes
	_waypoints = createWaypoints();
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

// Initializes nameToPtr and nameToVecPtr
void Segment::initializeMappings()
{
	nameToPtr[baseVisualName] =		&Segment::_baseVisual;
	nameToPtr[baseCollisionName] =	&Segment::_baseCollision;

	nameToVecPtr[waypointsName] =					&Segment::_waypointMeshes;
	nameToVecPtr[temperatureZoneVisualName] =		&Segment::_temperatureZoneVisuals;
	nameToVecPtr[temperatureZoneCollisionName] =	&Segment::_temperatureZoneCollisions;
	nameToVecPtr[boundingBoxName] =					&Segment::_boundingBoxMeshes;

	mappingsCreated = true;
}

// Iterate through meshes in scene and assign to proper pointers
void Segment::assignMeshPointers()
{
	std::string currentName;
	for (unsigned i = 0; i < _scene->mNumMeshes; ++i)
	{
		// currentName contains name of current mesh
		// readName will not have trailing digits removed, used for error logging
		std::string readName = currentName = _scene->mMeshes[i]->mName.C_Str();

		// Will hold the number at the end of the mesh name
		std::string trailingNumberString;
		int trailingNumber { -1 };

		// Remove any numbers at end of name
		while (isdigit(currentName[currentName.length() - 1]))
		{
			trailingNumberString.push_back(currentName[currentName.length() - 1]);
			currentName.pop_back();
		}

		if (trailingNumberString.length() > 0)
		{
			trailingNumber = atoi(trailingNumberString.data());
		}

		// If currentName is equal to a name mapped to an aiMesh*, 
		// set the current mesh to appropriate class member pointer
		if (nameToPtr.find(currentName) != nameToPtr.end())
		{
			this->*nameToPtr[currentName] = _scene->mMeshes[i];
		}
		// If currentName is equal to a name mapped to a vector<aiMesh*>, 
		// add the current mesh to the appropriate class member vector
		else if (nameToVecPtr.find(currentName) != nameToVecPtr.end())
		{
			// If mesh is numbered, place meshes in order
			if (trailingNumber >= 0)
			{
				(this->*nameToVecPtr[currentName]).reserve(trailingNumber);
				(this->*nameToVecPtr[currentName])[trailingNumber] = _scene->mMeshes[i];
			}
			// Else, place in read order
			else
			{
				(this->*nameToVecPtr[currentName]).push_back(_scene->mMeshes[i]);
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
std::vector<BoundingBox>&& Segment::createBoundingBoxes()
{
	std::vector<BoundingBox> boxes;

	// Create a bounding box for every mesh in _boundingBoxMeshes
	for (unsigned i = 0; i < _boundingBoxMeshes.size(); ++i)
	{
		// Check for correct number of faces
		if (_boundingBoxMeshes[i]->mNumFaces != 12)
		{
			LOG_ERROR("A bounding box mesh was not a box");
			continue;
		}
		
		BoundingBox box{};

		// Get middle of box
		box.center = findMeshCenter(_boundingBoxMeshes[i]);

		// Find first two directions and half-lengths of box
		findTwoDirections(_boundingBoxMeshes[i], box);

		box.halfLengths[2] = findThirdHalfDistance(_boundingBoxMeshes[i], box);
		if (box.halfLengths[2] < 0.0f)
		{
			LOG_ERROR("Could not find the third half-length of a bounding box");
		}
		else
		{
			// Box created successfully
			boxes.push_back(box);
		}
	}

	// Clear _boundingBoxMeshes to save memory
	_boundingBoxMeshes.clear();

	return std::move(boxes);
}

// Create waypoints from _waypointMeshes
std::vector<glm::vec3>&& Segment::createWaypoints()
{
	// Create vector of average positions, middle[0] is always (0, 0, 0)
	std::vector<glm::vec3> middles = createAverageWaypoints(_waypointMeshes);

	if (middles.size() < 2)
	{
		LOG_ERROR("Segment ", _segmentName, " has fewer than 2 waypoints. Not good :(");
		return move(std::vector<glm::vec3>{});
	}

	// Will accumulate the length between waypoints
	float length = 0.0f;

	// Set middle[i] to the waypoint closest to middle[i - 1]
	// middle[0] is always (0, 0, 0)
	for (unsigned i = 1; i < middles.size(); ++i)
	{
		// Index of closest, initialize to first position tested
		unsigned closest = i;

		// Squared distance from middles[i] to middles[i - 1], will hold distance from closest waypoint to middles[i - 1]
		float lowestDistance = squaredDistance(middles[i], middles[i - 1]);

		for (unsigned j = i + 1; j < middles.size(); ++j)
		{
			float testDistance = squaredDistance(middles[j], middles[i]);

			if (testDistance < lowestDistance)
			{
				lowestDistance = testDistance;
				closest = j;
			}
		}

		// Move closest point to position 0 in vector
		if (closest != i)
		{
			std::swap(middles[i], middles[closest]);
		}

		length += sqrtf(lowestDistance);
	}

	// Initialize _length
	_length = length;

	// Clear _waypointMeshes to save memory
	_waypointMeshes.clear();

	return std::move(middles);
}

// Create a vector with average positions of a vector of meshes, insert (0, 0, 0) at index 0
std::vector<glm::vec3>&& Segment::createAverageWaypoints(std::vector<aiMesh*>& meshes)
{
	// Create a vector containing the average position of all vertices in corresponding mesh in meshes
	// First waypoint is always (0, 0, 0)
	std::vector<glm::vec3> middles{ glm::vec3{0, 0, 0} };

	for (unsigned i = 0; i < meshes.size(); ++i)
	{
		// Initialize average position
		middles.push_back(glm::vec3{ 0, 0, 0 });
		const size_t index = middles.size() - 1;

		// Add the position of every vertex in the model
		for (unsigned j = 0; j < meshes[i]->mNumVertices; ++j)
		{
			middles[index].x += meshes[i]->mVertices[j].x;
			middles[index].y += meshes[i]->mVertices[j].y;
			middles[index].z += meshes[i]->mVertices[j].z;
		}

		// Divide by number of vertices to obtain average
		middles[index] /= meshes[i]->mNumVertices;
	}

	return std::move(middles);
}

// Find the center of a mesh
glm::vec3 Segment::findMeshCenter(aiMesh* mesh) const
{
	glm::vec3 center {0, 0, 0};

	// Add position of every vertex
	for (unsigned i = 0; i < mesh->mNumVertices; ++i)
	{
		center.x += mesh->mVertices[i].x;
		center.y += mesh->mVertices[i].y;
		center.z += mesh->mVertices[i].z;
	}

	// Divide by number of vertices and return
	return (center /= mesh->mNumVertices);
}

// Helper function for createBoundingBoxes(). Finds the third half-distance of a box
float Segment::findThirdHalfDistance(const aiMesh* boundingBoxMesh, const BoundingBox& box) const
{
	// Reduce initializer length
	aiVector3D* vertices = boundingBoxMesh->mVertices;
	aiFace* faces = boundingBoxMesh->mFaces;

	// Loop through faces to find an edge along the third direction. This will be used to find the third half-length
	for (unsigned j = 1; j < boundingBoxMesh->mNumFaces; ++j)
	{
		// Construct vertices for this face
		std::array<glm::vec3, 3> faceVertices{
			glm::vec3{ vertices[faces[j].mIndices[0]].x, vertices[faces[j].mIndices[0]].y, vertices[faces[j].mIndices[0]].z },
			glm::vec3{ vertices[faces[j].mIndices[1]].x, vertices[faces[j].mIndices[1]].y, vertices[faces[j].mIndices[1]].z },
			glm::vec3{ vertices[faces[j].mIndices[2]].x, vertices[faces[j].mIndices[2]].y, vertices[faces[j].mIndices[2]].z }
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
void Segment::findTwoDirections(const aiMesh* boundingBoxMesh, BoundingBox& box) const
{
	// Use to reduce firstFaceVertices initializer length
	aiFace* faces = boundingBoxMesh->mFaces;
	aiVector3D* vertices = boundingBoxMesh->mVertices;

	// Vertices of first face in bounding box, will be used to construct two direction vectors
	std::array<glm::vec3, 3> firstFaceVertices{
		glm::vec3{ vertices[faces[0].mIndices[0]].x, vertices[faces[0].mIndices[0]].y, vertices[faces[0].mIndices[0]].z },
		glm::vec3{ vertices[faces[0].mIndices[1]].x, vertices[faces[0].mIndices[1]].y, vertices[faces[0].mIndices[1]].z },
		glm::vec3{ vertices[faces[0].mIndices[2]].x, vertices[faces[0].mIndices[2]].y, vertices[faces[0].mIndices[2]].z }
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