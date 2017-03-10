#include "Core/Gui/GuiTrackGenerator.hpp"

GuiTrackGenerator::GuiTrackGenerator(SegmentHandler* segmentHandler, ObstacleHandler* obstacleHandler)
: _segmentHandler(segmentHandler)
, _obstacleHandler(obstacleHandler)
{
    _track.reset(new Track(_segmentHandler, _obstacleHandler));
    _preview.reset(new TrackPreview());
}

GuiTrackGenerator::~GuiTrackGenerator()
{
    _bDoAbortGenerate = true;
}


std::unique_ptr<Track> GuiTrackGenerator::takeTrack()
{
    if(_generation.valid())
    {
        auto pair = _generation.get();
        _track.swap(pair.first);
    }

    std::unique_ptr<Track> ret = std::move(_track);
    _track.reset(new Track(_segmentHandler, _obstacleHandler));
    _bHasTrack = false;

    return std::move(ret);
}

bool GuiTrackGenerator::bHasTrack() const
{
    return _bHasTrack;
}


void GuiTrackGenerator::render(GFX::RenderStates& states)
{
    _preview->render(states);
}

void GuiTrackGenerator::update(float dtSeconds)
{
    if(_generation.valid() && _generation.wait_for(std::chrono::milliseconds::zero()) == std::future_status::ready)
    {
        auto pair = _generation.get();
        _track.swap(pair.first);
        _preview->setTrack(*_track);
        _bHasTrack = true;
    }

    _preview->update(dtSeconds);
}

void GuiTrackGenerator::generate()
{

    if(_generation.valid())
    {
        _bDoAbortGenerate = true;
        _generation.get();
        _bDoAbortGenerate = false;
    }

    auto generationFunc = [this]()
    {
        auto nullReturn = std::make_pair(std::unique_ptr<Track>(nullptr), std::unique_ptr<TrackPreview>(nullptr));

        std::cout << "Generating... " << std::endl;
        auto track = std::unique_ptr<Track>(new Track(_segmentHandler, _obstacleHandler));
        track->setCurviness(_curviness);
        track->setSeed(_seed);
        track->setLength(_length);
        track->setDifficulty(_difficulty);
        track->startNewTrack();
        while(!_bDoAbortGenerate && !track->bGenerate());

        if(_bDoAbortGenerate)
        {
            std::cout << "Aborted." << std::endl;
            return nullReturn;
        }

//        auto preview = std::unique_ptr<TrackPreview>(new TrackPreview(*track));
        auto nullPreview = std::unique_ptr<TrackPreview>(nullptr);
        std::cout << "Done generating." << std::endl;
//        return nullReturn;
        return std::make_pair(std::move(track), std::move(nullPreview));
    };

    _generation = std::async(std::launch::async, generationFunc);
}

void GuiTrackGenerator::setLength(unsigned int length)
{
    _length = length;
}

void GuiTrackGenerator::setSeed(const std::string& seed)
{
    _seed = seed;
}

void GuiTrackGenerator::setCurviness(unsigned int curviness)
{
    _curviness = curviness;
}

void GuiTrackGenerator::setDifficulty(unsigned int difficulty)
{
    _difficulty = difficulty;
}
