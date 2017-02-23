#include "Core/Gui/GuiTrackGenerator.hpp"

GuiTrackGenerator::GuiTrackGenerator()
: _segmentHandler("Segments/segmentinfos4.txt", "Segments/ConnectionTypes.txt")
{
    _track.reset(new Track(&_segmentHandler));
    _preview.reset(new TrackPreview());

	for(size_t i = 0; i < _segmentHandler.infos().size(); i++)
    {
        _segmentHandler.loadSegment(i);
    }

    generate();
}


std::unique_ptr<Track> GuiTrackGenerator::takeTrack()
{
    std::unique_ptr<Track> ret = std::move(_track);
    _track.reset(new Track(&_segmentHandler));


    return std::move(ret);
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
        auto track = std::unique_ptr<Track>(new Track(&_segmentHandler));
        track->setCurviness(_curviness);
        track->setSeed(_seed);
        track->setLength(_length);
        track->startNewTrack();
        while(!_bDoAbortGenerate && !track->bGenerate())
        {
            std::cout << ".";
        }
        std::cout << std::endl;

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

    // Multithreading not safe as of this moment
    // because Track does OpenGL operations.
    _generation = std::async(std::launch::async, generationFunc);
//    auto pair = generationFunc();
//    _track.swap(pair.first);
//    _preview->setTrack(*_track);
}

void GuiTrackGenerator::setTrackAttributes()
{
	_track->setCurviness(_curviness);
	_track->setSeed(_seed);
	_track->setLength(_length);
}

void GuiTrackGenerator::setLength(unsigned int length)
{
    _length = length;
    generate();
}

void GuiTrackGenerator::setSeed(unsigned int seed)
{
    _seed = seed;
    generate();
}

void GuiTrackGenerator::setCurviness(unsigned int curviness)
{
    _curviness = curviness;
    generate();
}
