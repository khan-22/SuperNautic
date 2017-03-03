#pragma once
#ifndef SEGMENT_HPP
#define SEGMENT_HPP

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
#include "Core/Utility/Utilities.hpp"

struct WaypointInfo
{
	bool found{ false };
	glm::vec3 position;
	glm::vec3 direction;
	float distance;
	unsigned index;	// index in _waypoints
};

// Uninstantiated version of a track segment
class Segment
{
public:
	// Loads a segment from an fbx file
	Segment(const SegmentInfo* segmentInfo);

	// Tests a ray collision against all collision surfaces of the segment. Returns collision information
	// temperatures[i] holds temperature of temperature zone i
	const RayIntersection rayIntersectionTest(const Ray& ray, const std::vector<float>& temperatures) const;

	// Returns bounding boxes
	const std::vector<BoundingBox>& getBoundingBoxes() const;

	// Returns connection names
	char getStart() const;
	char getEnd() const;

	const SegmentInfo * getInfo() const;

	// Returns approximate segment length
	float getLength() const;

	const glm::mat4x4& getEndMatrix() const;

	const std::vector<glm::vec3>& getWaypoints() const;

	unsigned getNumZones() const;

	// Finds position of and distance to the waypoint closest to a position (position is relative to segment's local origin)
	WaypointInfo findClosestWaypoint(const glm::vec3& position) const;

	GFX::TexturedModel getVisualModel() const;

	ModelAsset	getWindowModel() const;

	bool bHasWindow() const;

private:
	const SegmentInfo*	_segmentInfo;

	// The loaded scene data
	RawMeshAsset _scene;

	// The visual geometry for this segment
	GFX::TexturedModel	_visual;
	ModelAsset			_windowModel;
	bool				_bHasWindow;
	ModelAsset			_zonesModel;
	bool				_bHasZones;

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

#endif // SEGMENT_H

