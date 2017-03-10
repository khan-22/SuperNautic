#pragma once

#ifndef GUI_TRACK_GENERATOR_HPP
#define GUI_TRACK_GENERATOR_HPP

#include <future>

#include "Core/Track/Track.hpp"
#include "Core/Gui/TrackPreview.hpp"
#include "Core/Track/ObstacleHandler.hpp"

class GuiTrackGenerator : public GFX::Renderable3D
{
    public:
        GuiTrackGenerator(SegmentHandler* segmentHandler, ObstacleHandler* obstacleHandler);
        ~GuiTrackGenerator();

        bool bHasTrack() const;
        std::unique_ptr<Track> takeTrack();
        void render(GFX::RenderStates& states) override;
        void update(float dtSeconds);
		void abortGeneration();


        void generate();
        void setLength(unsigned int length);
        void setSeed(const std::string& seed);
        void setCurviness(unsigned int curviness);
        void setDifficulty(unsigned int difficulty);

    private:
        bool _bDoAbortGenerate = false;
        bool _bHasTrack = false;
        std::future<std::pair<std::unique_ptr<Track>, std::unique_ptr<TrackPreview>>> _generation;
        SegmentHandler* _segmentHandler;
		ObstacleHandler* _obstacleHandler;
        std::unique_ptr<Track> _track;
        std::unique_ptr<TrackPreview> _preview;

        unsigned int _length = Track::_MIN_LENGTH;
        std::string	 _seed = "1";
        unsigned int _curviness = Track::_MAX_CURVINESS;
        unsigned int _difficulty = Track::_MIN_DIFFICULTY;

};

#endif //GUI_TRACK_GENERATOR_HPP
