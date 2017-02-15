#include <unordered_map>
#include <algorithm>
#include <limits>
#include <map>

#include "Core/Track/Segment.hpp"
#include "Core/Io/Log.hpp"

// Static constants declared in Segment.hpp
const std::string Segment::baseVisualName					{ "VM" };
const std::string Segment::baseCollisionName				{ "CM" };
const std::string Segment::temperatureZoneVisualName		{ "zoneVisual" };
const std::string Segment::temperatureZoneCollisionName		{ "zoneCollision" };
const std::string Segment::boundingBoxName					{ "BB" };
const std::string Segment::waypointsName					{ "WP" };

// Static maps declared in Segment.hpp
std::unordered_map<std::string, unsigned Segment::*> Segment::nameToIndex;
std::unordered_map<std::string, std::vector<unsigned> Segment::*> Segment::nameToVecIndex;

// Mappings are by default not created
bool Segment::mappingsCreated { false };


// Loads a segment from an fbx file
Segment::Segment(const SegmentInfo* segmentInfo)
{
	_segmentInfo = segmentInfo;

	// Get the scene data asset
	_scene = RawMeshCache::get(std::string{ "Segments/" } + _segmentInfo->_dataFileName);

	// Get visual model asset
	_visual = GFX::TexturedModel(ModelCache::get(std::string{ "Segments/" } +_segmentInfo->_visualFileName), MaterialCache::get("test3pipe.mat"));
		//ModelCache::get(std::string{ "Segments/" } + _segmentInfo->_visualFileName);

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
		LOG_ERROR("No meshes in file ", _segmentInfo->_dataFileName);
		return;
	}

	// Create bounding boxes from meshes
	createBoundingBoxes();

	// Create waypoints from meshes
	createWaypoints();

	// Create oct-tree
	createOctTree(20, 1);
}

// Tests a ray collision against all collision surfaces of the segment. Returns collision information
const RayIntersection Segment::rayIntersectionTest(const Ray& ray) const
{
	// Get distance to largest box
	float firstBoxDistance = _octTree.rayIntersection(ray);

	// If missed, return with miss
	if (firstBoxDistance < 0.0f)
	{
		return RayIntersection{ false };
	}
	else
	{
		// Keep relevant boxes in map ordered by distance to ensure that the first hit is always the closest
		std::multimap<float, const AABB*> boxesToTest;

		// Insert first box
		boxesToTest.insert(std::pair<float, const AABB*>(firstBoxDistance, &_octTree));

		// Search until a hit is found or all relevant boxes are checked
		while (boxesToTest.size() > 0)
		{
			// If box has children it contains no geometry, add children hit by ray to map
			if (boxesToTest.begin()->second->_children.size() > 0)
			{
				// For every child
				for (unsigned i = 0; i < boxesToTest.begin()->second->_children.size(); ++i)
				{
					// Check if child intersects ray
					float hitValue = boxesToTest.begin()->second->_children[i].rayIntersection(ray);

					// If hit, add child to map
					if (hitValue >= 0.0f)
					{
						boxesToTest.insert(std::pair<float, const AABB*>(hitValue, &(boxesToTest.begin()->second->_children[i])));
					}
				}
			}
			// Else the box contains geometry, check for triangle collision
			else
			{
				// Construct vector of indices of models
				std::vector<unsigned> modelIndices{ _temperatureZoneCollisions };
				modelIndices.push_back(_baseCollision);

				// Test intersection with geometry
				RayIntersection intersection = boxesToTest.begin()->second->triangleRayIntersection(ray, modelIndices, _scene);

				if (intersection)
				{
					intersection._normal = glm::normalize(intersection._normal);

					return intersection;
				}
			}

			// Remove checked box from map
			boxesToTest.erase(boxesToTest.begin());
		}

		// If no hit was found, return with miss
		return RayIntersection{ false };
	}
}

// Finds indices of the two waypoints closest to a position (position is relative to segment's local origin)
std::pair<WaypointInfo, WaypointInfo> Segment::findClosestWaypoints(const glm::vec3& position) const
{
	// Will hold the closest two vectors, initialize to first two waypoints
	// First is closer than second
	std::pair<unsigned, unsigned> closest { 0, 1 };

	float firstDistance = glm::distance(_waypoints[closest.first], position);
	float secondDistance = glm::distance(_waypoints[closest.second], position);

	// Swap if second is closer than first
	if (secondDistance < firstDistance)
	{
		std::swap(closest.first, closest.second);
		std::swap(firstDistance, secondDistance);
	}

	// Check rest of waypoints
	for (unsigned i = 2; i < _waypoints.size(); ++i)
	{
		float testDistance = glm::distance(_waypoints[i], position);

		if (testDistance < secondDistance)
		{
			// This is the closest waypoint so far, first becomes second, _waypoints[i] becomes first
			if (testDistance < firstDistance)
			{
				// Set first to second
				closest.second = closest.first;
				secondDistance = firstDistance;

				// Set test to first
				closest.first = i;
				firstDistance = testDistance;
			}
			// Second closest so far
			else
			{
				// Set test to second
				closest.second = i;
				secondDistance = testDistance;
			}
		}
	}

	// Make sure that first < second
	if (closest.first > closest.second)
	{
		std::swap(closest.first, closest.second);
		std::swap(firstDistance, secondDistance);
	}

	glm::vec3 firstDirection{ glm::normalize(_waypoints[closest.second] - _waypoints[closest.first]) };
	glm::vec3 secondDirection;

	// This is the last waypoint of the segment
	if (closest.second >= _waypoints.size() - 1)
	{
		glm::vec3 endPoint{ getEndMatrix() * glm::vec4{0, 0, 0, 1} };

		secondDirection = glm::normalize(endPoint - _waypoints[closest.second]);
	}
	else
	{
		secondDirection = glm::normalize(_waypoints[closest.second + 1] - _waypoints[closest.second]);
	}

	return std::make_pair(WaypointInfo{ _waypoints[closest.first], firstDirection, firstDistance }, WaypointInfo{ _waypoints[closest.second], secondDirection, secondDistance });
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
	// Data file
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
		LOG_ERROR("Mismatch: number of visual temperature models not equal to number of collision models in segment ", _segmentInfo->_visualFileName);
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
		LOG_ERROR("Segment ", _segmentInfo->_visualFileName, " has fewer than 2 waypoints. Not good :(");
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
		float lowestDistance = glm::distance(_waypoints[i], _waypoints[i - 1]);

		for (unsigned j = i + 1; j < _waypoints.size(); ++j)
		{
			float testDistance = glm::distance(_waypoints[j], _waypoints[i]);

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

		length += lowestDistance;
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

// Divides the collision geometry of this segment into an oct-tree
void Segment::createOctTree(unsigned maxFacesPerBox, unsigned maxSubdivisions)
{
	// Find the largest and smallest vertex position in each axis
	// and use as starting values for box

	// Initialize min and max
	glm::vec3 min = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
	glm::vec3 max = { std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min() };

	// Look through base model
	findMinMaxValues(min, max, _baseCollision);

	// Look through zones
	for (unsigned i = 0; i < _temperatureZoneCollisions.size(); ++i)
	{
		findMinMaxValues(min, max, _temperatureZoneCollisions[i]);
	}

	// Set values to corners of oct-tree root
	_octTree._minCorner = min;
	_octTree._maxCorner = max;

	// Will contain indices into vertices/faces vectors of models
	// [0]..[_temperatureZoneCollisions.size()] is zone collisions, [_temperatureZoneCollisions.size()] is base collision
	std::vector<std::vector<unsigned>> vertexIndices;
	std::vector<std::vector<unsigned>> faceIndices;

	// Fill indices
	for (size_t i = 0; i <= _temperatureZoneCollisions.size(); ++i)
	{
		// Create subvectors
		vertexIndices.emplace_back();
		faceIndices.emplace_back();

		// Number of vertices of current model
		size_t vertsToFill;

		// Number of faces of current model
		size_t facesToFill;

		// If filling zone collision
		if (i < _temperatureZoneCollisions.size())
		{
			vertsToFill = _scene.get()->meshes[_temperatureZoneCollisions[i]].vertices.size();
			facesToFill = _scene.get()->meshes[_temperatureZoneCollisions[i]].faces.size();
		}
		// Else, filling base collision
		else
		{
			vertsToFill = _scene.get()->meshes[_baseCollision].vertices.size();
			facesToFill = _scene.get()->meshes[_baseCollision].faces.size();
		}

		// Fill vertex indices
		for (unsigned j = 0; j < vertsToFill; ++j)
		{
			vertexIndices[i].push_back(j);
		}

		// Fill face indices
		for (unsigned j = 0; j < facesToFill; ++j)
		{
			faceIndices[i].push_back(j);
		}
	}

	// Subdivide
	subdivideOctTree(_octTree, maxFacesPerBox, maxSubdivisions, std::move(vertexIndices), std::move(faceIndices));
}

// Find smallest and largest vertex position in each axis for a model, using min and max for comparison
void Segment::findMinMaxValues(glm::vec3& min, glm::vec3& max, unsigned modelIndex)
{
	size_t numVerts = _scene.get()->meshes[modelIndex].vertices.size();
	for (size_t i = 0; i < numVerts; ++i)
	{
		// X
		if (_scene.get()->meshes[modelIndex].vertices[i].x < min.x)
		{
			min.x = _scene.get()->meshes[modelIndex].vertices[i].x;
		}
		if (_scene.get()->meshes[modelIndex].vertices[i].x > max.x)
		{
			max.x = _scene.get()->meshes[modelIndex].vertices[i].x;
		}

		// Y
		if (_scene.get()->meshes[modelIndex].vertices[i].y < min.y)
		{
			min.y = _scene.get()->meshes[modelIndex].vertices[i].y;
		}
		if (_scene.get()->meshes[modelIndex].vertices[i].y > max.y)
		{
			max.y = _scene.get()->meshes[modelIndex].vertices[i].y;
		}


		// Z
		if (_scene.get()->meshes[modelIndex].vertices[i].z < min.z)
		{
			min.z = _scene.get()->meshes[modelIndex].vertices[i].z;
		}
		if (_scene.get()->meshes[modelIndex].vertices[i].z > max.z)
		{
			max.z = _scene.get()->meshes[modelIndex].vertices[i].z;
		}
	}
}

// Recursively subdivides an AABB until every box touches <= maxFacesPerBox or maxSubdivisions == 0
void Segment::subdivideOctTree(AABB& box, unsigned maxFacesPerBox, unsigned maxSubdivisions, std::vector<std::vector<unsigned>>&& vertexIndices, std::vector<std::vector<unsigned>>&& faceIndices)
{
	bool stopSubdividing = false;

	// If maxSubdivisions == 0, stop subdividing, enter faces into this AABB, return
	if (maxSubdivisions == 0)
	{
		stopSubdividing = true;
	}
	// Check if total number of faces is less than max. If so, stop subdividing
	else
	{
		// Total number of faces
		size_t sumFaces = 0;

		for (size_t i = 0; i < faceIndices.size(); ++i)
		{
			sumFaces += faceIndices[i].size();
		}

		// If total faces <= maxFacesPerBox, stop subdividing
		if (sumFaces <= maxFacesPerBox)
		{
			stopSubdividing = true;
		}
	}

	if (stopSubdividing)
	{
		// Enter faces into box and return
		box.faceIndices = std::move(faceIndices);
		return;
	}

	// Create 8 new boxes, vertex/face index vector for them, and vertex index movement vector
	std::vector<AABB> possibleChildren = createChildren(box._minCorner, box._maxCorner);

	// Vectors corresponding to vertexIndices and faceIndices for every child
	std::vector<std::vector<std::vector<unsigned>>> childrenVertexIndices(8);
	std::vector<std::vector<std::vector<unsigned>>> childrenFaceIndices(8);

	// Initialize subvectors
	for (unsigned i = 0; i < 8; ++i)
	{
		for (unsigned j = 0; j < _temperatureZoneCollisions.size() + 1; ++j)  // +1 to include _baseCollison
		{
			childrenVertexIndices[i].emplace_back();
			childrenFaceIndices[i].emplace_back();
		}
	}

	// For every vertex, this vector contains an index representing which child the index occupies
	std::vector<std::vector<unsigned>> vertexMovement(vertexIndices.size());


	// For every vertex, check which child it belongs to, copy to corresponding vertex index, indicate move in vertex index movement vector

	// Middle of box, used to compare vertices
	glm::vec3 boxMiddle { (box._minCorner + box._maxCorner) * 0.5f };

	// For every model
	for (size_t i = 0; i < vertexIndices.size(); ++i)
	{
		size_t currentModel;

		if (i < vertexIndices.size() - 1)
		{
			currentModel = _temperatureZoneCollisions[i];
		}
		else
		{
			currentModel = _baseCollision;
		}

		// For every vertex in model
		for (size_t j = 0; j < vertexIndices[i].size(); ++j)
		{
			// Index into possibleChildren
			unsigned targetChild = findChildIndex(currentModel, vertexIndices[i][j], boxMiddle);

			// Pass index to correct child
			childrenVertexIndices[targetChild][i].push_back(vertexIndices[i][j]);

			// Log movement
			vertexMovement[i].push_back(targetChild);
		}

		// For every face, copy to all children containing a vertex
		for (size_t j = 0; j < faceIndices[i].size(); ++j)
		{
			// Indicates the children this face has been copied to (prevents multiple copies)
			int placedIn1 = -1;
			int placedIn2 = -1;

			// For every vertex in face
			for (unsigned k = 0; k < 3; ++k)
			{
				// The index of the vertex that needs to be found
				size_t indexToFind = _scene.get()->meshes[currentModel].faces[j][k];

				// Find the vertex index in array and calculate its index
				auto foundAt = std::lower_bound(vertexIndices[i].begin(), vertexIndices[i].end(), indexToFind);
				ptrdiff_t indexInMovementVector = foundAt - vertexIndices[i].begin();

				unsigned targetChild = vertexMovement[i][indexInMovementVector];

				// Copy face to child if not already copied
				if (placedIn1 < 0)
				{
					placedIn1 = targetChild;
					childrenFaceIndices[targetChild][i].push_back(faceIndices[i][j]);
				}
				else if (placedIn1 != targetChild)
				{
					if (placedIn2 != targetChild)
					{
						placedIn2 = targetChild;
						childrenFaceIndices[targetChild][i].push_back(faceIndices[i][j]);
					}
				}
			}
		}
	}

	// For every child with nonempty index vectors, copy into AABB children vector and call this function with corresponding vertexIndices and faceIndices
	for (unsigned i = 0; i < possibleChildren.size(); ++i)
	{
		// A child will only be kept if it contains geometry
		bool childContainsData = false;

		// Check for geometry from every mesh
		for (unsigned j = 0; j < childrenFaceIndices[i].size(); ++j)
		{
			if (childrenFaceIndices[i][j].size() > 0)
			{
				childContainsData = true;
				break;
			}
		}

		// Add child to box and possibly subdivide it if it contains data
		if (childContainsData)
		{
			box._children.push_back(possibleChildren[i]);
			subdivideOctTree(box._children[box._children.size() - 1], maxFacesPerBox, maxSubdivisions - 1, std::move(childrenVertexIndices[i]), std::move(childrenFaceIndices[i]));
		}
	}
}

// Helper for subdivideOctTree, creates child boxes from a parent
// Copy corners by value to reduce cost of constantly referencing
std::vector<AABB> Segment::createChildren(glm::vec3 min, glm::vec3 max)
{
	std::vector<AABB> possibleChildren
	{
			AABB{ glm::vec3{ min }, glm::vec3{ (min + max) * 0.5f } },	// nX nY nZ
			AABB{ glm::vec3{ min.x, min.y, (min.z + max.z) * 0.5f }, glm::vec3{ (min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, max.z } },	// nX nY pZ
			AABB{ glm::vec3{ min.x, (min.y + max.y) * 0.5f, min.z }, glm::vec3{ (min.x + max.x) * 0.5f, max.y, (min.z + max.z) * 0.5f } },	// nX pY nZ
			AABB{ glm::vec3{ min.x, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f }, glm::vec3{ (min.x + max.x) * 0.5f, max.y, max.z } },	// nX pY pZ
			AABB{ glm::vec3{ (min.x + max.x) * 0.5f, min.y, min.z }, glm::vec3{ max.x, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f } },	// pX nY nZ
			AABB{ glm::vec3{ (min.x + max.x) * 0.5f, min.y, (min.z + max.z) * 0.5f }, glm::vec3{ max.x, (min.y + max.y) * 0.5f, max.z } },	// pX nY pZ
			AABB{ glm::vec3{ (min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, min.z }, glm::vec3{ max.x, max.y, (min.z + max.z) * 0.5f } },	// pX pY nZ
			AABB{ glm::vec3{ (min + max) * 0.5f }, glm::vec3{ max } }	// pX pY pZ
	};

	return possibleChildren;
}

// Helper for subdivideOctTree, finds index of child to copy a vertex to
unsigned Segment::findChildIndex(size_t currentModel, unsigned index, glm::vec3 boxMiddle)
{
	unsigned targetChild = 0;

	// Find correct index
	if (_scene.get()->meshes[currentModel].vertices[index].x > boxMiddle.x)
	{
		targetChild += 4;
	}

	if (_scene.get()->meshes[currentModel].vertices[index].y > boxMiddle.y)
	{
		targetChild += 2;
	}

	if (_scene.get()->meshes[currentModel].vertices[index].z > boxMiddle.z)
	{
		targetChild += 1;
	}

	return targetChild;
}