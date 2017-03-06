#pragma once

#ifndef APPLICATION_CONTEXT_HPP
#define APPLICATION_CONTEXT_HPP

namespace sf
{
    class RenderWindow;
}

#include "Core/Track/SegmentHandler.hpp"
#include "Core/Track/ObstacleHandler.hpp"
#include "Core/Track/Track.hpp"
#include "Core/Gui/GuiPlayerJoinContainer.hpp"
#include "Core/Gui/MenuBackground.hpp"
#include "Core/Audio/WorldAudio.hpp"

struct ApplicationContext
{
    ApplicationContext(sf::RenderWindow& window)
    : window(window)
    {
    }

    sf::RenderWindow& window;

    // These should probably be turned into assets...
    std::unique_ptr<SegmentHandler> segmentHandler;
    std::unique_ptr<ObstacleHandler> obstacleHandler;

    std::unique_ptr<Track> track;

    std::vector<GuiPlayerJoinContainer::Player> players;

    std::unique_ptr<MenuBackground> menuBackground;

	WorldAudio audio;
};


#endif //APPLICATION_CONTEXT_HPP
