#pragma once
#ifndef DEBUGCAMERA_HPP
#define DEBUGCAMERA_HPP

#include <SFML/Window.hpp>

#include "Core/Utility/Camera.h"

class DebugCamera : public Camera {
 public:
  DebugCamera(float fov, int viewWidth, int viewHeight,
              const glm::vec3& pos = glm::vec3(0.0, 0.0, 0.0),
              const glm::vec3& viewDirection = glm::vec3(0.0, 0.0, 0.0));
  virtual ~DebugCamera();
  void update(const float dt, sf::Window& window);

 private:
  DebugCamera();

  float _anglePitch;
  float _angleYaw;
  float _moveSpeed;
  bool _bActive;
};

#endif  // !DEBUGCAMERA_HPP
