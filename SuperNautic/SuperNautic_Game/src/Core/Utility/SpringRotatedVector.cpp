#include <algorithm>

#include <glm/gtx/vector_angle.hpp>
#include "SpringRotatedVector.hpp"
#include "Utilities.hpp"

SpringRotatedVector::SpringRotatedVector(const glm::vec3& vector,
                                         const glm::vec3& target,
                                         const glm::vec3& backupAxis,
                                         float springConstant,
                                         float dampingConstant, bool locked)
    : _vector{glm::normalize(vector)},
      _target{glm::normalize(target)},
      _backupAxis{glm::normalize(backupAxis)},
      _springConstant{springConstant},
      _dampingConstant{dampingConstant},
      _lockedToAxis{locked} {
  if (_lockedToAxis) {
    // Force orthogonalization of target via setTarget function
    setTarget(glm::normalize(target));
  }
}

void SpringRotatedVector::update(float dt) {
  glm::vec3 newAxis = glm::normalize(glm::cross(_vector, _target));

  // Find angle between vectors
  float angleBetween;
  float dot = glm::dot(_vector, _target);
  if (dot >= 0.999f) {
    angleBetween = 0.0f;
    newAxis = _backupAxis;
  } else if (dot <= -0.999f) {
    angleBetween = glm::pi<float>();
    newAxis = _backupAxis;
  } else {
    angleBetween = acosf(dot);
  }

  if (!std::isnan(newAxis.x)) {
    // Update rotation axis and velocity (represented by length)
    _axis += glm::normalize(newAxis) * angleBetween * angleBetween *
             _springConstant * dt;

    if (_lockedToAxis) {
      _axis = glm::dot(_axis, _backupAxis) * _backupAxis;
    }
  }

  // Reduce velocity (represented by axis length) using damping constant
  _axis -= _axis * _dampingConstant * dt;

  // Rotate vector
  if (!bAlmostEqual(_axis, glm::vec3{0, 0, 0})) {
    float len = glm::length(_axis);
    float angle = len * dt;
    glm::vec3 normalized = glm::normalize(_axis);

    glm::mat4 rotation = glm::rotate(angle, normalized);

    _vector = glm::normalize(rotation * glm::vec4{_vector, 0.0f});

    if (_lockedToAxis) {
      _vector -= glm::dot(_vector, _backupAxis) * _backupAxis;
    }
  }
}

const glm::vec3& SpringRotatedVector::operator()() const { return _vector; }

const glm::vec3& SpringRotatedVector::getVector() const { return (*this)(); }

void SpringRotatedVector::setTarget(const glm::vec3& target) {
  if (!_lockedToAxis) {
    _target = glm::normalize(target);
  } else {
    _target =
        glm::normalize(target - glm::dot(target, _backupAxis) * _backupAxis);
  }
}

void SpringRotatedVector::setBackupAxis(const glm::vec3& backupAxis) {
  _backupAxis = glm::normalize(backupAxis);
}

void SpringRotatedVector::setSpringConstant(float springConstant) {
  _springConstant = springConstant;
}

void SpringRotatedVector::setDampingConstant(float dampingConstant) {
  _dampingConstant = dampingConstant;
}

void SpringRotatedVector::setVector(const glm::vec3& vector) {
  _vector = vector;
}
