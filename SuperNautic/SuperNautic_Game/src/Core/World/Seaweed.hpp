#ifndef SEAWEED_HPP
#define SEAWEED_HPP

#include <glm/glm.hpp>

#include "Core/Asset/LoadAssetFunctions.hpp"
#include "GFX/Rendering/Renderable3D.hpp"
#include "GFX/Resources/TexturedModel.hpp"

class Seaweed : public GFX::Renderable3D {
 public:
  Seaweed(glm::vec3 position);
  void render(GFX::RenderStates& states) override;
  const glm::vec3& getPosition() const;

 private:
  glm::vec3 _position;
  GFX::TexturedModel _seaweedModel;
  glm::mat4 _seaweedTransform;
};

#endif  // !SEAWEED_HPP
