#include "Core/Gui/GuiTrackGenerator.hpp"



GuiTrackGenerator::GuiTrackGenerator()
: _segmentHandler("Segments/segmentinfos3.txt", "Segments/ConnectionTypes.txt")
{
    _track = std::unique_ptr<Track>(new Track(&_segmentHandler));
    setTrackAttributes();

    generate();
}


std::unique_ptr<Track> GuiTrackGenerator::takeTrack()
{
    std::unique_ptr<Track> ret = std::move(_track);
    _track = std::unique_ptr<Track>(new Track(&_segmentHandler));
    setTrackAttributes();

    return std::move(ret);
}


void GuiTrackGenerator::render(GFX::RenderStates& states)
{
    _preview.render(states);
}

void GuiTrackGenerator::update(float dtSeconds)
{
    _preview.update(dtSeconds);
}

void GuiTrackGenerator::generate()
{
	_track->startNewTrack();
	while(!_track->bGenerate());
	_preview.setTrack(*_track);
}

void GuiTrackGenerator::setTrackAttributes()
{
	_track->setCurviness(_curviness);
	_track->setSeed(_seed);
	_track->setLength(_length);
}

void GuiTrackGenerator::setLength(unsigned int length)
{
    _track->setLength(length);
    generate();
}

void GuiTrackGenerator::setSeed(unsigned int seed)
{
    _track->setSeed(seed);
    generate();
}

void GuiTrackGenerator::setCurviness(unsigned int curviness)
{
    _track->setCurviness(curviness);
    generate();
}
