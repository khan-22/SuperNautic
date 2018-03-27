#pragma once

#ifndef SPRINGTRANSLATEDVECTOR_HPP
#define SPRINGTRANSLATEDVECTOR_HPP

#include <glm/glm.hpp>

class SpringTranslatedVector {
 public:
  SpringTranslatedVector(const glm::vec3& vector, const glm::vec3& target,
                         const glm::vec3& alternateAxis, float springConstant,
                         float alternateSpringConstant);
  void update(float dt);

  const glm::vec3& operator()() const;

  const glm::vec3& getVector() const;

  void setTarget(const glm::vec3& target);

  void setSpringConstant(float springConstant);

  void setAlternateSpringConstant(float alternateSpringConstant);

  void setVector(const glm::vec3& vector);

  void setAlternateAxis(const glm::vec3& slowAxis);

 private:
  // The translating vector
  glm::vec3 _vector;
  // The translation target
  glm::vec3 _target;
  // The current velocity
  glm::vec3 _velocity;
  // The vector moves at a different speed along this axis
  glm::vec3 _alternateAxis;
  // Velocity along alternate axis
  glm::vec3 _alternateVelocity;

  // Force of spring
  float _springConstant;
  float _alternateSpringConstant;
};

#endif