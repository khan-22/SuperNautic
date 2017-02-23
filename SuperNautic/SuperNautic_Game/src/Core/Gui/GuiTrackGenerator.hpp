#pragma once

#ifndef GUI_TRACK_GENERATOR_HPP
#define GUI_TRACK_GENERATOR_HPP

#include <future>

#include "Core/Track/Track.hpp"
#include "Core/Gui/TrackPreview.hpp"


class GuiTrackGenerator : public GFX::Renderable3D
{
    public:
        GuiTrackGenerator();

        std::unique_ptr<Track> takeTrack();
        void render(GFX::RenderStates& states) override;
        void update(float dtSeconds);

        void setLength(unsigned int length);
        void setSeed(unsigned int seed);
        void setCurviness(unsigned int curviness);

    private:
        bool _bDoAbortGenerate = false;
        std::future<std::pair<std::unique_ptr<Track>, std::unique_ptr<TrackPreview>>> _generation;
        SegmentHandler _segmentHandler;
        std::unique_ptr<Track> _track;
        std::unique_ptr<TrackPreview> _preview;

        unsigned int _length = 40000;
        unsigned int _seed = 1;
        unsigned int _curviness = 3;

        void generate();
        void setTrackAttributes();
};

#endif //GUI_TRACK_GENERATOR_HPP
