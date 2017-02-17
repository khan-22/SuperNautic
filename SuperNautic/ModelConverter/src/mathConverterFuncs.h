#pragma once

#ifndef MATH_CONVERTER_FUNCS_H
#define MATH_CONVERTER_FUNCS_H

#include <assimp/scene.h>
#include <glm/glm.hpp>

glm::vec3 toGLM(aiVector3D vec);

glm::mat4 toGLM(aiMatrix4x4 mat);

#endif