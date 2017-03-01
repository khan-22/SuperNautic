#pragma once

#ifndef WINDOW_CONSTANTS_HPP
#define WINDOW_CONSTANTS_HPP

#include "SFML/Window/ContextSettings.hpp"

namespace WindowConstants
{
    constexpr char* const TITLE = "SuperNautic";
    constexpr unsigned int NUM_DEPTH_BITS = 24;
    constexpr unsigned int NUM_STENCIL_BITS = 8;
    constexpr unsigned int ANTIALIASING_LEVEL = 0;
    constexpr unsigned int CONTEXT_MAJOR_VERSION = 4;
    constexpr unsigned int CONTEXT_MINOR_VERSION = 0;
    const sf::ContextSettings CONTEXT_SETTINGS
    (
        NUM_DEPTH_BITS,
        NUM_STENCIL_BITS,
        ANTIALIASING_LEVEL,
        CONTEXT_MAJOR_VERSION,
        CONTEXT_MINOR_VERSION
    );
}

#endif //WINDOW_CONSTANTS_HPP
