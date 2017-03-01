#pragma once

#ifndef VIDEO_OPTIONS_HPP
#define VIDEO_OPTIONS_HPP

#include "SFML/Window/Window.hpp"


#include "glm/glm.hpp"

#include "Core/Io/ConfigFile.hpp"

class VideoOptions
{
public:
    VideoOptions(sf::Window& window);

    void recreateWindow();
    void setResolution(size_t x, size_t y);
    void setFullscreen(bool bSetFullscreen);
    bool bIsFullscreen() const;

    static const std::vector<glm::ivec2>& getAllowedResolutions();
    static bool bIsResolutionAllowed(size_t x, size_t y);

private:
    static constexpr char* const _CONFIG_PATH = "config.cfg";
    static constexpr uint32_t _DEFAULT_WINDOW_STYLE = sf::Style::Titlebar | sf::Style::Close;
    static const std::vector<glm::ivec2> _ALLOWED_RESOLUTIONS;
    static size_t _DEFAULT_RESOLUTION_X;
    static size_t _DEFAULT_RESOLUTION_Y;

    sf::Window& _window;
    ConfigFile  _config;

    uint32_t    _windowStyle;
    size_t      _resolutionX;
    size_t      _resolutionY;

    void readConfig();
    void rebuildConfig();
    void writeConfig();
};


#endif //VIDEO_OPTIONS_HPP
