#pragma once

#ifndef VIDEO_OPTIONS_HPP
#define VIDEO_OPTIONS_HPP

#include "SFML/Window/Window.hpp"

#include <glm/glm.hpp>

#include "Core/Io/ConfigFile.hpp"

class VideoOptions {
 public:
  VideoOptions(sf::Window& window);

  void recreateWindow();
  void setResolution(const glm::ivec2& resolution);
  const glm::ivec2& getResolution() const;
  void setFullscreen(bool bSetFullscreen);
  bool bIsFullscreen() const;

  static const std::vector<glm::ivec2>& getAllowedResolutions();
  static bool bIsResolutionAllowed(const glm::ivec2& resolution);

 private:
  static const char* const _TITLE;
  static const unsigned int _NUM_DEPTH_BITS;
  static const unsigned int _NUM_STENCIL_BITS;
  static const unsigned int _ANTIALIASING_LEVEL;
  static const unsigned int _CONTEXT_MAJOR_VERSION;
  static const unsigned int _CONTEXT_MINOR_VERSION;
  static const sf::ContextSettings _CONTEXT_SETTINGS;

  static const char* const _CONFIG_PATH;
  static const bool _B_IS_FULLSCREEN;
  static const std::vector<glm::ivec2> _ALLOWED_RESOLUTIONS;
  static const glm::ivec2 _DEFAULT_RESOLUTION;

  sf::Window& _window;
  ConfigFile _config;

  bool _bIsFullscreen;
  glm::ivec2 _resolution;

  void readConfig();
  void rebuildConfig();
  void writeConfig();
};

#endif  // VIDEO_OPTIONS_HPP
