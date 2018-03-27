#pragma once

#ifndef DIRLIGHT_H
#define DIRLIGHT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

struct DirLightProperties {
  glm::vec3 direction;
  glm::vec3 ambientColor;
  glm::vec3 diffuseColor;

  GLfloat intensity;
};

class DirLight {
 public:
  DirLight(glm::vec3 direction, glm::vec3 ambientColor, glm::vec3 diffuseColor,
           GLfloat intensity);

  void update(GLfloat dt);
  void changeIntensity(GLfloat newIntensity, GLfloat transitionTime);

  const DirLightProperties getLightProperties();

 private:
  glm::vec3 _direction;
  glm::vec3 _ambientColor;
  glm::vec3 _diffuseColor;

  GLfloat _currentIntensity;
  GLfloat _newIntensity;
  GLfloat _previousIntensity;
  GLfloat _transitionTimer;
  GLfloat _transitionTotalTime;

  DirLight();
};

#endif