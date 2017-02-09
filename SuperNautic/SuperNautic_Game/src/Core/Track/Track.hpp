#pragma once
#ifndef TRACK_HPP
#define TRACK_HPP

#include <vector>
#include <glm\mat4x4.hpp>

#include "SegmentHandler.hpp"
#include "Segment.hpp"
#include "SegmentInstance.hpp"

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
	SegmentInstance& getInstance(const int index);

private:
	Track();
	int getIndex(char & connectionType) const;
	int getInRow(const int index) const;
	bool insertNormalSegment(const int index, int & length, bool testCollision);
	void insertStructure(const int index, int & length);
	void deleteSegments(int & totalLength, const int lengthToDelete);
	
	SegmentHandler *				_segmentHandler;
	std::vector<SegmentInstance*>	_track;
	int								_targetLength;
	int								_generatedLength;
	unsigned int					_seed;
	glm::mat4						_endMatrix;
};

#endif // !TRACK_HPP
