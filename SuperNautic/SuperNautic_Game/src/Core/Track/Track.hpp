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

class Track
{
public:
	Track(SegmentHandler * segmentHandler);
	virtual ~Track();
	int getTargetLength() const;
	int getGeneratedLength() const;
	void setLength(const unsigned int length);
	void setSeed(const unsigned int seed);
	bool generate();
	int getNrOfSegments() const;
	SegmentInstance* getInstance(int index);
	void render(GFX::DeferredRenderer& renderer);

	// Returns the forward vector for a given ship position and segment index (segment index may update)
	glm::vec3 findForward(const glm::vec3 globalPosition, unsigned& segmentIndex);
	//SegmentInstance& getInstance(const int index);

private:
	Track();
	int getIndex(char connectionType) const;
	int getInRow(const int index) const;
	bool bInsertNormalSegment(const int index, int & length, bool testCollision);
	void insertStructure(const int index, int & length);
	void deleteSegments(int & totalLength, const int lengthToDelete);
	bool bEndTrack(int & totalLength);

	SegmentHandler *				_segmentHandler;
	std::vector<SegmentInstance*>	_track;
	const int						_endMargin;
	int								_targetLength;
	int								_generatedLength;
	unsigned int					_seed;
	glm::mat4						_endMatrix;
};

#endif // !TRACK_HPP
