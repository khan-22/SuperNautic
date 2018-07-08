#pragma once

#include "Core/Asset/LoadAssetFunctions.hpp"

struct Font
{
    FontAsset font = FontCache::get("res/arial.ttf");
    unsigned int char_size = 30;
    sf::Color fill_color = sf::Color::White;
    sf::Color outline_color = sf::Color::White;
    float outline_thickness = 0.f;
};
