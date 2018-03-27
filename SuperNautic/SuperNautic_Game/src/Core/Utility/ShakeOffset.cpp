#include "ShakeOffset.hpp"
#include <algorithm>
#include <glm/gtc/constants.hpp>

#include "Core/Io/Log.hpp"

ShakeOffset::ShakeOffset(float horizontalSpeed, float horizontalMagnitude,
                         float verticalSpeed, float verticalMagnitude,
                         float decaySpeed, float baseShakeSpeed)
    : _horizontalSpeed{horizontalSpeed},
      _horizontalMagnitude{horizontalMagnitude},
      _verticalSpeed{verticalSpeed},
      _verticalMagnitude{verticalMagnitude},
      _baseSpeed{baseShakeSpeed},
      _decaySpeed{decaySpeed},
      _hAccumulator{0.0f},
      _vAccumulator{0.0f},
      _shakeMagnitude{0.0f},
      _shakeSpeed{0.0f} {}

void ShakeOffset::update(float dt) {
  _hAccumulator += (_baseSpeed + _shakeSpeed) * _horizontalSpeed * dt;
  _vAccumulator += (_baseSpeed + _shakeSpeed) * _verticalSpeed * dt;

  // Reset accumulator value to prevent accuracy problems
  float threshold = glm::pi<float>() * 2.0f;
  while (_hAccumulator > threshold && threshold > 0.0f) {
    _hAccumulator -= threshold;
  }

  while (_vAccumulator > threshold && threshold > 0.0f) {
    _vAccumulator -= threshold;
  }

  // Decay shake values
  if (_shakeMagnitude > 0.0f) {
    _shakeMagnitude = std::max(0.0f, _shakeMagnitude - _decaySpeed * dt);
  }

  if (_shakeSpeed > 0.0f) {
    _shakeSpeed = std::max(0.0f, _shakeSpeed - _decaySpeed * dt);
  }
}

glm::vec3 ShakeOffset::getVector() const {
  return glm::vec3{sinf(_hAccumulator) * _horizontalMagnitude * _shakeMagnitude,
                   sinf(_vAccumulator) * _verticalMagnitude * _shakeMagnitude,
                   0.0f};
}

glm::vec3 ShakeOffset::operator()() const { return getVector(); }

void ShakeOffset::setMagnitude(float shakeMagnitude) {
  if (shakeMagnitude > _shakeMagnitude) {
    _shakeMagnitude = shakeMagnitude;
  }
}

void ShakeOffset::setSpeed(float shakeSpeed) {
  if (shakeSpeed > _shakeSpeed) {
    _shakeSpeed = shakeSpeed;
  }
}

void ShakeOffset::setDecaySpeed(float decaySpeed) { _decaySpeed = decaySpeed; }
