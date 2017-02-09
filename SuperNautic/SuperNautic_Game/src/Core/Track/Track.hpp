#pragma once
#ifndef TRACK_HPP
#define TRACK_HPP

#include <vector>
#include <list>
#include <glm\mat4x4.hpp>

#include "SegmentHandler.hpp"
#include "Segment.hpp"
#include "SegmentInstance.hpp"
#include "../../GFX/DeferredRenderer.hpp"
#include "../../GFX/ForwardRenderer.hpp"
#include "../../GFX/Box.hpp"

class Track
{
public:
	Track(SegmentHandler * segmentHandler);
	virtual ~Track();
	int getTargetLength() const;
	int getGeneratedLength() const;
	bool setLength(const int length);
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
	int getIndex(char & connectionType) const;
	int getInRow(const int index) const;
	bool insertNormalSegment(const int index, int & length, bool testCollision);
	void insertStructure(const int index, int & length);
	void deleteSegments(int & totalLength, const int lengthToDelete);
	void generateObstacles();


	SegmentHandler *				_segmentHandler;
	std::vector<SegmentInstance*>	_track;
	int								_targetLength;
	int								_generatedLength;
	unsigned int					_seed;
	glm::mat4						_endMatrix;
	std::list<GFX::Box> _obstacles;
};

#endif // !TRACK_HPP
