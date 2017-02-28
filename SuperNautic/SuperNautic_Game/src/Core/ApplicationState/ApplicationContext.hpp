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

    unsigned char numPlayers;
};


#endif //APPLICATION_CONTEXT_HPP
