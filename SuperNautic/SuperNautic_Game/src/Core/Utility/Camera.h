#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
 public:
  Camera(float fov, int viewWidth, int viewHeight,
         const glm::vec3& pos = glm::vec3(0.0, 0.0, 0.0),
         const glm::vec3& viewDirection = glm::vec3(0.0, 0.0, 0.0),
         const glm::vec3& up = glm::vec3(0.0, 0.0, 0.0));
  virtual ~Camera();
  glm::vec3 getPosition() const;
  glm::vec3 getDirection() const;
  glm::vec3 getUp() const;
  glm::mat4 getView() const;
  void setView(const glm::mat4& view);
  glm::mat4 getPerspective() const;
  glm::mat4 getVP() const;
  void setPos(const glm::vec3& newPos);
  void setPosAged(const glm::vec3& newPos);
  void move(const glm::vec3& moveVec);
  void setUp(const glm::vec3& newUp);
  void setViewDir(const glm::vec3& newDir);
  void setViewDirAged(const glm::vec3& newDir);
  void setFOV(float newFOV);
  void changeFOV(float deltaFOV);
  void setAspectRatio(int width, int height);

 protected:
  Camera();

  glm::mat4 _perspective;
  glm::mat4 _view;
  glm::vec3 _pos;
  glm::vec3 _up;
  glm::vec3 _viewDir;

  float _fov;
  int _viewWidth;
  int _viewHeight;
};

#endif  // !CAMERA_H
