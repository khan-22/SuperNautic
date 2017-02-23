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

class Track
{
public:
	Track(SegmentHandler * segmentHandler);
	virtual ~Track();
	int getTargetLength() const;
	int getGeneratedLength() const;
	void setLength(const unsigned int length);
	void setSeed(const unsigned int seed);
	void setCurviness(const unsigned int curviness);
	void startNewTrack();
	bool bGenerate();
	int getNrOfSegments() const;
	SegmentInstance* getInstance(int index);
	float getProgression() const;
	void render(GFX::DeferredRenderer& renderer, GFX::WindowRenderer& windowRenderer, const int shipIndex);

	// Returns the forward vector for a given ship position and segment index (segment index may update). Returns appropriate respawn position in returnPos and length from start of segment
	glm::vec3 findForward(const glm::vec3 globalPosition, unsigned& segmentIndex, glm::vec3& returnPos, float& lengthInSegment);

private:
	int getIndex() const;
	int getInRow(const int index) const;

	WaypointInfo findNextWaypointInfo(const WaypointInfo& current, unsigned segmentIndex) const;
	bool bInsertNormalSegment(const int index, bool testCollision);
	void insertStructure(const int index);
	void deleteSegments(const int lengthToDelete);
	bool bEndTrack();

	SegmentHandler *				_segmentHandler;
	std::vector<SegmentInstance*>	_track;
	unsigned int					_seed;
	float							_curviness;
	int								_targetLength;
	int								_generatedLength;
	float							_totalProgress;
	int								_lengthAfterLastCall;
	int								_progressionLength;
	const int						_endMargin;
	glm::mat4						_endMatrix;
	char							_endConnection;
	int								_prevIndex;
	const Segment *					_lastSegment;
	std::vector<GFX::Window>		_segmentWindows;
};

#endif // !TRACK_HPP
