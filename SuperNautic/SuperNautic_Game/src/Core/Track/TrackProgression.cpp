#include "Core/Track/TrackProgression.hpp"

TrackProgression::TrackProgression(unsigned startSegment, Track* track)
{
	_accumulatedLength = 0.0f;
	_currentSegment = startSegment;
	_track = track;
	_progress = 0.0f;
}

unsigned TrackProgression::getCurrentSegment() const
{
	return _currentSegment;
}

void TrackProgression::setCurrentSegment(unsigned segmentIndex)
{

	if (segmentIndex > _currentSegment)
	{
		_accumulatedLength += _track->getInstance(_currentSegment)->getLength();
		_currentSegment = segmentIndex;
	}
}

float TrackProgression::getProgression() const
{
	return _progress;
}

void TrackProgression::update(float lengthInSegment)
{
	_progress = (_accumulatedLength + lengthInSegment) / _track->getGeneratedLength();
}