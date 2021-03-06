#include <algorithm>
#include <glm\gtx\transform.hpp>

#include "Core/Utility/Camera.h"
#include "Core/Utility/Utilities.hpp"

//Default constructor
Camera::Camera()
{

}

//Real constructor
Camera::Camera(float fov, int viewWidth, int viewHeight, const glm::vec3 & pos
				, const glm::vec3 & viewDirection, const glm::vec3 & up)
	: _pos(pos)
	, _up(0.f, 1.f, 0.f)
	, _fov(fov)
	, _viewWidth(viewWidth)
	, _viewHeight(viewHeight)
	, _viewDir(viewDirection)
{
	_perspective = glm::perspective(glm::radians(_fov), (float)_viewWidth / _viewHeight, 0.1f, 1000.f);
	_view = glm::lookAt(_pos, _pos + _viewDir, _up);
}

//Destructor
Camera::~Camera()
{
	//Nothing
}

glm::vec3 Camera::getPosition() const
{
	return _pos;
}

glm::vec3 Camera::getDirection() const
{
	return _viewDir;
}

glm::vec3 Camera::getUp() const
{
	return _up;
}

//Returns the view matrix
glm::mat4 Camera::getView() const
{
	return _view;
}

//Returns the perspective matrix
glm::mat4 Camera::getPerspective() const
{
	return _perspective;
}

//Returns the view-perspective-matrix
glm::mat4 Camera::getVP() const
{
	return _perspective * _view;
}

//Sets a new absolute position
void Camera::setPos(const glm::vec3 & newPos)
{
	_pos = newPos;

	_view = glm::lookAt(_pos, _pos + _viewDir, _up);
}

//Sets the camera position with aging
void Camera::setPosAged(const glm::vec3 & newPos)
{
	_pos = (_pos + newPos) * 0.5f;
	_view = glm::lookAt(_pos, _pos + _viewDir, _up);
}

//Moves the camera from its current position
void Camera::move(const glm::vec3 & moveVec)
{
	_pos += moveVec;
	_view = glm::lookAt(_pos, _pos + _viewDir, _up);
}

//Sets the up vector
void Camera::setUp(const glm::vec3 & newUp)
{
	_up = newUp;
	_view = glm::lookAt(_pos, _pos + _viewDir, _up);
}

//Sets the view direction
void Camera::setViewDir(const glm::vec3 & newDir)
{
	_viewDir = newDir;

	_view = glm::lookAt(_pos, _pos + glm::normalize(_viewDir), _up);
}

//Sets the view dir with aging
void Camera::setViewDirAged(const glm::vec3 & newDir)
{
	_viewDir = (_viewDir + newDir) * 0.5f;
	_view = glm::lookAt(_pos, _pos + _viewDir, _up);
}

//Sets the field of view
void Camera::setFOV(float newFOV)
{
	_fov = newFOV;
	_perspective = glm::perspective(glm::radians(_fov), (float)_viewWidth / _viewHeight, 0.1f, 1000.f);
}

//Changes the field of view relative to its current value
void Camera::changeFOV(float deltaFOV)
{
	_fov += deltaFOV;
	_perspective = glm::perspective(glm::radians(_fov), (float)_viewWidth / _viewHeight, 0.1f, 1000.f);
}

void Camera::setAspectRatio(int width, int height)
{
	_viewWidth  = width;
	_viewHeight = height;

	_perspective = glm::perspective(glm::radians(_fov), (float)_viewWidth / _viewHeight, 0.1f, 1000.f);
}

void Camera::setView(const glm::mat4& view)
{
	_view = view;
}

