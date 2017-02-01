#pragma once
#ifndef SEGMENT_H
#define SEGMENT_H

#include <string>
#include <array>
#include <vector>
#include <unordered_map>

#include "glm/glm.hpp"
#include "../LoadAssetFunctions.hpp"

enum class SurfaceType : char { normal, hot, cold };

// Bounding box enclosing the segment
struct BoundingBox
{
	glm::vec3				center;

	// x, y, z directions
	std::array<glm::vec3, 3>	directions;

	// x, y, z halflengths
	std::array<float, 3>	halfLengths;

	BoundingBox() : center{ glm::vec3{} },
		halfLengths{ -1.0f, -1.0f, -1.0f }
	{ }
};

// Holds information about a detected collision
struct Intersection
{
	glm::vec3 position;
	glm::vec3 normal;
	SurfaceType surface;
};

// Uninstantiated version of a track segment
class Segment
{
public:
	const std::string _segmentName;

	// Loads a segment from an fbx file
	Segment(std::string dataFilePath, std::string visualFilePath, std::string startConnection, std::string endConnection);

	// Tests a ray collision against all collision surfaces of the segment. Returns collision information
	const Intersection rayIntersectionTest(glm::vec3 origin, glm::vec3 direction) const;

	// Finds the two waypoints closest to a position (position is relative to segment's local origin)
	std::pair<glm::vec3, glm::vec3> findClosestWaypoints(const glm::vec3& position) const;

	// Renders the segment at the position of an instance
	// TODO

	// Returns bounding boxes
	const std::vector<BoundingBox>& getBoundingBoxes() const
	{
		return _boundingBoxes;
	}

	// Returns connection names
	const std::string& getStart() const
	{
		return _startConnection;
	}
	const std::string& getEnd() const
	{
		return _endConnection;
	}

	// Returns approximate sement length
	float getLength() const
	{
		return _length;
	}

	const glm::mat4x4& getEndMatrix() const
	{
		return _scene.get()->cameras[0];
	}

private:
	// The loaded scene data
	RawMeshAsset _scene;

	// The visual geometry for this segment
	ModelAsset _visual;

	// Indices in _scene.get()->meshes to the required meshes
	unsigned				_baseVisual;
	unsigned				_baseCollision;
	std::vector<unsigned>	_temperatureZoneVisuals;
	std::vector<unsigned>	_temperatureZoneCollisions;
	std::vector<unsigned>	_boundingBoxMeshes;
	std::vector<unsigned>	_waypointMeshes;

	// Bounding boxes read from mesh
	std::vector<BoundingBox> _boundingBoxes;

	// Waypoints, read from mesh and sorted to the correct order
	std::vector<glm::vec3> _waypoints;

	// Names of start and end connections
	std::string _startConnection;
	std::string _endConnection;

	// Approximate length of segment, obtained from waypoints
	float _length;

	// Names of meshes expected in fbx file (defined in Segment.cpp)
	static const std::string baseVisualName;
	static const std::string baseCollisionName;
	static const std::string temperatureZoneVisualName;
	static const std::string temperatureZoneCollisionName;
	static const std::string boundingBoxName;
	static const std::string waypointsName;
	static const std::string endName;

	// Mappings from mesh name to aiMesh* or vector<aiMesh*>
	static std::unordered_map<std::string, unsigned Segment::*> nameToIndex;
	static std::unordered_map<std::string, std::vector<unsigned> Segment::*> nameToVecIndex;

	// True if nameToIndex and nameToVecIndex are initialized
	static bool mappingsCreated;

	// Initializes nameToIndex and nameToVecIndex
	void initializeMappings();

	// Iterate through meshes in scene and assign to proper pointers
	void assignMeshPointers();

	// Create bounding boxes from _boundingBoxMeshes
	void createBoundingBoxes();

	// Create waypoints from _waypointMeshes
	void createWaypoints();

	// Create a vector with average positions of a vector of mesh indices, insert (0, 0, 0) at index 0
	void createAverageWaypoints(std::vector<unsigned>& meshIndices);

	// Find the center of mesh _scene->meshes[meshIndex]
	glm::vec3 findMeshCenter(unsigned meshIndex) const;

	// True if two vectors are almost equal
	bool bAlmostEqual(glm::vec3 vector1, glm::vec3 vector2) const
	{
		return (0.0001f > abs(vector1.x - vector2.x)) &&
			   (0.0001f > abs(vector1.y - vector2.y)) &&
			   (0.0001f > abs(vector1.z - vector2.z));
	}

	// Helper function for createBoundingBoxes(). Finds the third half-distance of a box
	float findThirdHalfDistance(unsigned boundingBoxMeshIndex, const BoundingBox& box) const;

	// Helper function for createBoundingBoxes(). Finds three directions and two half-lengths of a box using the first face of the box mesh
	void findTwoDirections(unsigned boundingBoxMeshIndex, BoundingBox& box) const;
};

// Get squared distance between two vectors
inline float squaredDistance(const glm::vec3& vector1, const glm::vec3& vector2)
{
	float dX = vector1.x - vector2.x;
	float dY = vector1.y - vector2.y;
	float dZ = vector1.z - vector2.z;
	return dX * dX + dY * dY + dZ * dZ;
}

#endif // SEGMENT_H
