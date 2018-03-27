#include "Core/Geometry/BoundingBox.hpp"
#include <glm/vec3.hpp>
#include "Core/Io/Log.hpp"
#include "Core/Utility/Utilities.hpp"

BoundingBox::BoundingBox(const GFX::RawVertexData& model) {
  // Check for correct number of faces
  if (model.faces.size() != 12) {
    LOG_ERROR("The bounding box mesh is not a box!");
    abort();
  }

  // Get middle of box
  findMeshCenter(model);

  // Find first two directions and half-lengths of box
  findTwoDirections(model);

  findThirdHalfDistance(model);
  if (halfLengths[2] <= 0.0f) {
    LOG_ERROR("Could not find the third half-length of a bounding box");
    abort();
  }
}

void BoundingBox::findMeshCenter(const GFX::RawVertexData& model) {
  glm::vec3 foundCenter{0, 0, 0};

  // Add position of every vertex
  for (unsigned i = 0; i < model.vertices.size(); ++i) {
    foundCenter += model.vertices[i];
  }

  // Divide by number of vertices and set center
  float divisor = 1.0f / model.vertices.size();
  center = foundCenter * divisor;
}

void BoundingBox::findTwoDirections(const GFX::RawVertexData& model) {
  // Vertices of first face in bounding box, will be used to construct two
  // direction vectors
  std::array<glm::vec3, 3> firstFaceVertices{
      glm::vec3{model.vertices[model.faces[0].x]},
      glm::vec3{model.vertices[model.faces[0].y]},
      glm::vec3{model.vertices[model.faces[0].z]}};

  // Initialize three vectors, only two will be kept (the longest is not part of
  // a box edge)
  directions[0] = firstFaceVertices[0] - firstFaceVertices[1];
  float length0 = glm::length(directions[0]);
  directions[1] = firstFaceVertices[0] - firstFaceVertices[2];
  float length1 = glm::length(directions[1]);
  directions[2] = firstFaceVertices[1] - firstFaceVertices[2];
  float length2 = glm::length(directions[2]);

  // Swap vectors so that box.directions[2] is the longest, as it will be
  // replaced
  if (length2 < length0) {
    std::swap(directions[2], directions[0]);
    std::swap(length2, length0);
  }
  if (length2 < length1) {
    std::swap(directions[2], directions[1]);
    std::swap(length2, length1);
  }

  // Normalize first two vectors
  directions[0] = glm::normalize(directions[0]);
  directions[1] = glm::normalize(directions[1]);

  // The third vector is the cross product of the first two
  directions[2] = glm::cross(directions[0], directions[1]);

  // length0 and length1 are halved to find halfLengths of x and y
  halfLengths[0] = length0 / 2.0f;
  halfLengths[1] = length1 / 2.0f;
}

void BoundingBox::findThirdHalfDistance(const GFX::RawVertexData& model) {
  // Loop through faces to find an edge along the third direction. This will be
  // used to find the third half-length
  for (unsigned j = 1; j < model.faces.size(); ++j) {
    // Construct vertices for this face
    std::array<glm::vec3, 3> faceVertices{
        glm::vec3{model.vertices[model.faces[j].x]},
        glm::vec3{model.vertices[model.faces[j].y]},
        glm::vec3{model.vertices[model.faces[j].z]}};

    // Construct vectors between vertices of this face
    std::array<glm::vec3, 3> faceVectors{
        glm::vec3{faceVertices[0] - faceVertices[1]},
        glm::vec3{faceVertices[0] - faceVertices[2]},
        glm::vec3{faceVertices[1] - faceVertices[2]}};

    // If a vector points in the same or opposite direction as the third box
    // direction, the distance can be found from its length
    for (unsigned k = 0; k < 3; ++k) {
      if (bAlmostEqual(glm::normalize(faceVectors[k]), directions[2]) ||
          bAlmostEqual(glm::normalize(-faceVectors[k]), directions[2])) {
        halfLengths[2] = glm::length(faceVectors[k]) / 2.0f;
        break;
      }
    }
  }
}
