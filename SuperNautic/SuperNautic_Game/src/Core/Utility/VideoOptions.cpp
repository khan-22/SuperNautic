#include "Core/Utility/VideoOptions.hpp"
#include "Core/Utility/WindowConstants.hpp"
#include "Core/Utility/FileUtility.hpp"


size_t VideoOptions::_DEFAULT_RESOLUTION_X = 1280;
size_t VideoOptions::_DEFAULT_RESOLUTION_Y = 720;

const std::vector<glm::ivec2> VideoOptions::_ALLOWED_RESOLUTIONS =
{
    glm::ivec2(1920, 1080),
    glm::ivec2(1280, 720)
};

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
        !_config.get("resX", resolutionX) ||
        !_config.get("resY", resolutionY) ||
        !_config.get("fullscreen", bFullscreen)
    )
    {
        rebuildConfig();
        readConfig();
        return;
    }

    setResolution(resolutionX, resolutionY);
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
    if(!bIsResolutionAllowed(x, y))
    {
        return;
    }
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


const std::vector<glm::ivec2>& VideoOptions::getAllowedResolutions()
{
    return _ALLOWED_RESOLUTIONS;
}

bool VideoOptions::bIsResolutionAllowed(size_t x, size_t y)
{
    for(const glm::ivec2& res : _ALLOWED_RESOLUTIONS)
    {
        if(res.x == x && res.y == y)
        {
            return true;
        }
    }
    return false;
}
