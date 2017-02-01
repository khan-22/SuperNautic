#pragma once
#ifndef TRACK_HPP
#define TRACK_HPP

#include <vector>
#include <glm\glm.hpp>

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

private:
	Track();
	
	SegmentHandler *				_segmentHandler;
	std::vector<SegmentInstance>	_track;
	int								_targetLength;
	int								_generatedLength;
	unsigned int					_seed;
	glm::mat4						_endMatrix;
};

#endif // !TRACK_HPP
