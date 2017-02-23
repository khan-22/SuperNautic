#pragma once
#ifndef TRACK_HPP
#define TRACK_HPP

#include <vector>
#include <glm\mat4x4.hpp>

#include "Core/Track/SegmentHandler.hpp"
#include "Core/Track/Segment.hpp"
#include "Core/Track/SegmentInstance.hpp"
#include "GFX/Rendering/DeferredRenderer.hpp"
#include "GFX/Rendering/ForwardRenderer.hpp"
#include "GFX/Rendering/WindowRenderer.hpp"
#include "GFX/Resources/Window.hpp"
#include "Core/Geometry/Octree.hpp"
#include "Core/Track/ObstacleHandler.hpp"

class Track
{
public:
	Track(SegmentHandler * segmentHandler, ObstacleHandler * obstacleHandler);
	virtual ~Track();
	int getTargetLength() const;
	int getGeneratedLength() const;
	void setLength(const unsigned int length);
	void setSeed(const std::string& seed);
	const std::string& getSeed() const;
	void setCurviness(const unsigned int curviness);
	void startNewTrack();
	bool bGenerate();
	int getNrOfSegments() const;
	SegmentInstance* getInstance(int index);
	float getProgression() const;
	void render(GFX::DeferredRenderer& renderer, GFX::WindowRenderer& windowRenderer, const int shipIndex);
	void update(const float dt);

	// Returns the forward vector for a given ship position and segment index (segment index may update). Returns appropriate respawn position in returnPos and length from start of segment
	glm::vec3 Track::findForward(const glm::vec3 globalPosition, unsigned& segmentIndex, glm::vec3& returnPos, float& lengthInSegment, glm::vec3& directionDifference);

private:
	int getIndex() const;
	int getInRow(const int index) const;

	WaypointInfo findNextWaypointInfo(const WaypointInfo& current, unsigned segmentIndex) const;
	bool bInsertNormalSegment(const int index, bool testCollision);
	void insertStructure(const int index);
	void deleteSegments(const int lengthToDelete);
	bool bEndTrack();
	void placeObstacles();
	size_t findTrackIndex(const float totalLength, float & lastFullSegmentLength) const;

	std::vector<GFX::Window>				_segmentWindows;
	SegmentHandler *						_segmentHandler;
	ObstacleHandler *						_obstacleHandler;
	std::vector<SegmentInstance*>			_track;
	std::vector<Octree<SegmentInstance*>>	_octrees;
	std::string								_seed;
	float									_curviness;
	float									_difficulty;
	float									_targetLength;
	float									_generatedLength;
	float									_totalProgress;
	int										_lengthAfterLastCall;
	int										_progressionLength;
	const int								_endMargin;
	glm::mat4								_endMatrix;
	char									_endConnection;
	int										_prevIndex;
	const Segment *							_lastSegment;
};

#endif // !TRACK_HPP
