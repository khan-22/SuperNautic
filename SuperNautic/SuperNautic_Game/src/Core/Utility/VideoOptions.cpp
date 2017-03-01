#include "Core/Utility/VideoOptions.hpp"
#include "Core/Utility/WindowConstants.hpp"
#include "Core/Utility/FileUtility.hpp"


size_t VideoOptions::_DEFAULT_RESOLUTION_X = 1280;
size_t VideoOptions::_DEFAULT_RESOLUTION_Y = 720;

VideoOptions::VideoOptions(sf::Window& window)
: _window(window)
{
    readConfig();
}


void VideoOptions::readConfig()
{
    createFileIfNotExists(_CONFIG_PATH);
    _config.read(_CONFIG_PATH);
    size_t resolutionX;
    size_t resolutionY;
    bool bFullscreen;

    if
    (
        !_config.get("resX", _resolutionX) ||
        !_config.get("resY", _resolutionY) ||
        !_config.get("fullscreen", bFullscreen)
    )
    {
        rebuildConfig();
        readConfig();
        return;
    }

    setFullscreen(bFullscreen);
}

void VideoOptions::rebuildConfig()
{
    _config.set("resX", _DEFAULT_RESOLUTION_X);
    _config.set("resY", _DEFAULT_RESOLUTION_Y);
    _config.set("fullscreen", bool(_DEFAULT_WINDOW_STYLE & sf::Style::Fullscreen));
    _config.write(_CONFIG_PATH);
}

void VideoOptions::writeConfig()
{
    _config.set("resX", _resolutionX);
    _config.set("resY", _resolutionY);
    _config.set("fullscreen", bIsFullscreen());
    _config.write(_CONFIG_PATH);
}

void VideoOptions::recreateWindow()
{
    _window.create(sf::VideoMode(_resolutionX, _resolutionY), WindowConstants::TITLE, _windowStyle, WindowConstants::CONTEXT_SETTINGS);
}


void VideoOptions::setResolution(size_t x, size_t y)
{
    _resolutionX = x;
    _resolutionY = y;
    writeConfig();
}

void VideoOptions::setFullscreen(bool bSetFullscreen)
{
    if(bSetFullscreen)
    {
        _windowStyle |= uint32_t(sf::Style::Fullscreen);
    }
    else
    {
        _windowStyle &= ~uint32_t(sf::Style::Fullscreen);
    }

    writeConfig();
}

bool VideoOptions::bIsFullscreen() const
{
    return _windowStyle & sf::Style::Fullscreen;
}

