#pragma once

#ifndef GUI_TRACK_GENERATOR_HPP
#define GUI_TRACK_GENERATOR_HPP

#include "Core/Track/Track.hpp"


#include "Core/Gui/GuiElement.hpp"



class GuiTrackGenerator : public GuiElement
{
    public:
        GuiTrackGenerator();

        void setSize(float width, float height);

        sf::FloatRect getBoundingRect() const override;

        std::unique_ptr<Track> takeTrack();

    private:
        std::unique_ptr<Track> _track;

};

#endif //GUI_TRACK_GENERATOR_HPP
