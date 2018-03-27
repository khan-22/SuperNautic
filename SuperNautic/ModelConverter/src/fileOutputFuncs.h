#pragma once

#ifndef FILE_OUTPUT_FUNCS_H
#define FILE_OUTPUT_FUNCS_H

#include <GL/glew.h>
#include <stdint.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#define HEADER_RESERVED 128
#define HEADER_RESERVED_BYTES sizeof(uint32_t) * HEADER_RESERVED

struct Header {
 public:
  uint32_t magicNumber = 149;
  uint32_t numMeshes = 0;
  uint32_t numCameras = 0;
  uint32_t numLights = 0;
};

struct MeshHeader {
  uint32_t numVertices = 0;
  uint32_t numFaces = 0;
  uint32_t nameLength = 0;
};

struct Mesh {
  std::string name;
  unsigned int textureIndex = 0;

  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> texCoords;
  std::vector<glm::vec3> normals;
  std::vector<glm::uvec3> faces;
  std::vector<GLuint> indices;
};

struct Light {
  glm::vec4 color;
};

struct Camera {
  glm::mat4 transform;
};

static_assert(sizeof(Header) <= HEADER_RESERVED_BYTES,
              "Header has exceeded reserved bytes!");

void writeHeader(Header* header, FILE* file);
void writeHeader(MeshHeader* header, FILE* file);

void writeData(Mesh* mesh, FILE* file);
void writeData(Light* light, FILE* file);
void writeData(Camera* camera, FILE* file);

#endif