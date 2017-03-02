#include "Core/Utility/VideoOptions.hpp"
#include "Core/Utility/FileUtility.hpp"

const char* const VideoOptions::_TITLE = "SuperNautic";
const unsigned int VideoOptions::_NUM_DEPTH_BITS = 24;
const unsigned int VideoOptions::_NUM_STENCIL_BITS = 8;
const unsigned int VideoOptions::_ANTIALIASING_LEVEL = 0;
const unsigned int VideoOptions::_CONTEXT_MAJOR_VERSION = 4;
const unsigned int VideoOptions::_CONTEXT_MINOR_VERSION = 0;
const sf::ContextSettings VideoOptions::_CONTEXT_SETTINGS
(
    _NUM_DEPTH_BITS,
    _NUM_STENCIL_BITS,
    _ANTIALIASING_LEVEL,
    _CONTEXT_MAJOR_VERSION,
    _CONTEXT_MINOR_VERSION
);

const char* const VideoOptions::_CONFIG_PATH = "config.cfg";
const bool VideoOptions::_B_IS_FULLSCREEN = false;
const std::vector<glm::ivec2> VideoOptions::_ALLOWED_RESOLUTIONS =
{
    glm::ivec2(1280, 720),
    glm::ivec2(1920, 1080)
};
const glm::ivec2 VideoOptions::_DEFAULT_RESOLUTION = _ALLOWED_RESOLUTIONS[0];

VideoOptions::VideoOptions(sf::Window& window)
: _window(window)
{
    readConfig();
}


void VideoOptions::readConfig()
{
    createFileIfNotExists(_CONFIG_PATH);
    _config.read(_CONFIG_PATH);
    glm::ivec2 resolution;
    bool bFullscreen;

    if
    (
        !_config.get("resX", resolution.x) ||
        !_config.get("resY", resolution.y) ||
        !_config.get("fullscreen", bFullscreen)
    )
    {
        rebuildConfig();
        readConfig();
        return;
    }

    setResolution(resolution);
    setFullscreen(bFullscreen);
}

void VideoOptions::rebuildConfig()
{
    _config.set("resX", _DEFAULT_RESOLUTION.x);
    _config.set("resY", _DEFAULT_RESOLUTION.y);
    _config.set("fullscreen", _B_IS_FULLSCREEN);
    _config.write(_CONFIG_PATH);
}

void VideoOptions::writeConfig()
{
    _config.set("resX", _resolution.x);
    _config.set("resY", _resolution.y);
    _config.set("fullscreen", _bIsFullscreen);
    _config.write(_CONFIG_PATH);
}

void VideoOptions::recreateWindow()
{
    uint32_t style = _bIsFullscreen ? sf::Style::Fullscreen : sf::Style::Titlebar | sf::Style::Close;
    _window.create(sf::VideoMode(_resolution.x, _resolution.y), _TITLE, style, _CONTEXT_SETTINGS);
}


void VideoOptions::setResolution(const glm::ivec2& resolution)
{
    if(!bIsResolutionAllowed(resolution))
    {
        return;
    }
    _resolution = resolution;
    writeConfig();
}

void VideoOptions::setFullscreen(bool bSetFullscreen)
{
    _bIsFullscreen = bSetFullscreen;
    writeConfig();
}

bool VideoOptions::bIsFullscreen() const
{
    return _bIsFullscreen;
}


const std::vector<glm::ivec2>& VideoOptions::getAllowedResolutions()
{
    return _ALLOWED_RESOLUTIONS;
}

bool VideoOptions::bIsResolutionAllowed(const glm::ivec2& resolution)
{
    return std::find(_ALLOWED_RESOLUTIONS.begin(), _ALLOWED_RESOLUTIONS.end(), resolution) != _ALLOWED_RESOLUTIONS.end();
}

const glm::ivec2& VideoOptions::getResolution() const
{
    return _resolution;
}
