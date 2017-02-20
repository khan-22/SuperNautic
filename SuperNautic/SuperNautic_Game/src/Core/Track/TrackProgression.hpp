#pragma once

#ifndef TRACKPROGRESSION_H
#define TRACKPROGRESSION_H

#include "Core/Track/Track.hpp"

class TrackProgression
{
public:
	TrackProgression(unsigned startSegment, Track* track);
	unsigned getCurrentSegment() const;
	void setCurrentSegment(unsigned segmentIndex);

	float getProgression() const;

	float update(float lengthInSegment);
private:
	unsigned _currentSegment;

	// [0..1]
	float _progress;

	float _accumulatedLength;

	Track* _track;
};

#endif