#pragma once
#ifndef SEGMENT_H
#define SEGMENT_H

#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <memory>

#include "glm/glm.hpp"

#include "Core/Asset/LoadAssetFunctions.hpp"
#include "Core/Geometry/AABB.hpp"
#include "Core/Geometry/BoundingBox.hpp"
#include "Core/Geometry/RayIntersection.hpp"
#include "Core/Geometry/Ray.hpp"
#include "Core/Track/SegmentInfo.hpp"
#include "GFX/Resources/TexturedModel.hpp"

struct WaypointInfo
{
	glm::vec3 position;
	glm::vec3 direction;
	float distance;
};

// Uninstantiated version of a track segment
class Segment
{
public:
	const SegmentInfo*	_segmentInfo;

	// Loads a segment from an fbx file
	Segment(const SegmentInfo* segmentInfo);

	// Tests a ray collision against all collision surfaces of the segment. Returns collision information
	const RayIntersection rayIntersectionTest(const Ray& ray) const;

	// Renders the segment at the position of an instance
	// TODO

	// Returns bounding boxes
	const std::vector<BoundingBox>& getBoundingBoxes() const
	{
		return _boundingBoxes;
	}

	// Returns connection names
	char getStart() const
	{
		return _segmentInfo->_startConnection;
	}
	char getEnd() const
	{
		return _segmentInfo->_endConnection;
	}

	// Returns "probability"
	int getProbability() const
	{
		return _segmentInfo->_probability;
	}

	int getMinInRow() const
	{
		return _segmentInfo->_minInRow;
	}

	int getMaxInRow() const
	{
		return _segmentInfo->_maxInRow;
	}

	int getRotationOffset() const
	{
		return _segmentInfo->_rotationOffset;
	}

	// Returns approximate segment length
	float getLength() const
	{
		return _length;
	}

	const glm::mat4x4& getEndMatrix() const
	{
		return _scene.get()->cameras[0];
	}

	const std::vector<glm::vec3>& getWaypoints() const
	{
		return _waypoints;
	}

	// Finds positions of and distance to the two waypoints closest to a position (position is relative to segment's local origin)
	// First in pair is before second
	std::pair<WaypointInfo, WaypointInfo> findClosestWaypoints(const glm::vec3& position) const;

	GFX::TexturedModel getVisualModel() const
	{
		return _visual;
	}

private:
	// The loaded scene data
	RawMeshAsset _scene;

	// The visual geometry for this segment
	GFX::TexturedModel _visual;

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

	// Approximate length of segment, obtained from waypoints
	float _length;

	// Root of oct-tree for collision geometry
	AABB _octTree;

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

	// Helper function for createBoundingBoxes(). Finds the third half-distance of a box
	float findThirdHalfDistance(unsigned boundingBoxMeshIndex, const BoundingBox& box) const;

	// Helper function for createBoundingBoxes(). Finds three directions and two half-lengths of a box using the first face of the box mesh
	void findTwoDirections(unsigned boundingBoxMeshIndex, BoundingBox& box) const;

	// Divides the collision geometry of this segment into an oct-tree
	void createOctTree(unsigned maxFacesPerBox, unsigned maxSubdivisions);

	// Find smallest and largest vertex position in each axis for a model, using min and max for comparison
	void findMinMaxValues(glm::vec3& min, glm::vec3& max, unsigned modelIndex);

	// Recursively subdivides an AABB until every box touches <= maxFacesPerBox or maxSubdivisions == 0
	void subdivideOctTree(AABB& box, unsigned maxFacesPerBox, unsigned maxSubdivisions, std::vector<std::vector<unsigned>>&& vertexIndices, std::vector<std::vector<unsigned>>&& faceIndices);

	// Helper for subdivideOctTree, creates child boxes from a parent
	std::vector<AABB> createChildren(glm::vec3 min, glm::vec3 max);

	// Helper for subdivideOctTree, finds index of child to copy a vertex to
	unsigned findChildIndex(size_t currentModel, unsigned index, glm::vec3 boxMiddle);
};

// True if two vectors are almost equal
inline bool bAlmostEqual(glm::vec3 vector1, glm::vec3 vector2)
{
	return (0.0001f > abs(vector1.x - vector2.x)) &&
		(0.0001f > abs(vector1.y - vector2.y)) &&
		(0.0001f > abs(vector1.z - vector2.z));
}

#endif // SEGMENT_H

