#include <cstdlib>

#include "Seaweed.hpp"

#include <glm/glm.hpp>

Seaweed::Seaweed(glm::vec3 position)
    : _position{position},
      _seaweedModel{ModelCache::get("seaweed.kmf"),
                    MaterialCache::get("seaweed.mat")} {
  _seaweedTransform =
      glm::translate(_position) *
      glm::rotate(static_cast<float>(rand()) / RAND_MAX, glm::vec3{0, 1, 0});
}

void Seaweed::render(GFX::RenderStates& states) {
  _seaweedModel.getModelAsset().get()->setModelMatrix(_seaweedTransform);
  _seaweedModel.render(states);
}

const glm::vec3& Seaweed::getPosition() const { return _position; }
