#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Core/Asset/LoadAsset.hpp"
#include "GFX/Resources/Loaders/MaterialLoader.hpp"
#include "GFX/Resources/Loaders/ModelLoader.hpp"
#include "GFX/Resources/Loaders/ShaderLoader.hpp"
#include "GFX/Resources/Loaders/VertexDataImporter.hpp"
#include "GFX/Resources/Texture.hpp"

// Load raw mesh/vertex data
template <>
std::shared_ptr<GFX::RawMeshCollection> loadAsset<GFX::RawMeshCollection>(
    std::string key) {
  GFX::VertexDataImporter importer("./res/models/");
  return std::shared_ptr<GFX::RawMeshCollection>(
      importer.importVertexData(key));
}

// Load shaders
template <>
std::shared_ptr<GFX::Shader> loadAsset<GFX::Shader>(std::string key) {
  GFX::ShaderLoader loader("./src/GFX/Shaders/");
  return std::shared_ptr<GFX::Shader>(loader.loadShader(key));
}

// Load fonts
template <>
std::shared_ptr<sf::Font> loadAsset<sf::Font>(std::string key) {
  auto font = std::make_shared<sf::Font>();
  if (font->loadFromFile(key)) {
    return font;
  } else {
    return nullptr;
  }
}

// Load WAV
template <>
std::shared_ptr<sf::SoundBuffer> loadAsset<sf::SoundBuffer>(std::string key) {
  auto buffer = std::make_shared<sf::SoundBuffer>();
  if (buffer->loadFromFile("./res/audio/" + key + ".wav")) {
    return buffer;
  } else {
    return nullptr;
  }
}

// Load SFML Texture
template <>
std::shared_ptr<sf::Texture> loadAsset<sf::Texture>(std::string key) {
  auto texture = std::make_shared<sf::Texture>();
  if (texture->loadFromFile("./res/textures/" + key)) {
    return texture;
  } else {
    return nullptr;
  }
}

// Load models
template <>
std::shared_ptr<GFX::Model> loadAsset<GFX::Model>(std::string key) {
  GFX::ModelLoader loader;
  return std::shared_ptr<GFX::Model>(loader.loadModel(key));
}

// Load Texture from file.
template <>
std::shared_ptr<GFX::Texture> loadAsset<GFX::Texture>(std::string key) {
  sf::Image img;
  if (!img.loadFromFile("./res/textures/" + key)) {
    return nullptr;
  }

  sf::Vector2u size = img.getSize();
  return std::make_shared<GFX::Texture>(img.getPixelsPtr(), size.x, size.y,
                                        GL_TEXTURE_2D);
}

// Load Material from file.
template <>
std::shared_ptr<GFX::Material> loadAsset<GFX::Material>(std::string key) {
  return GFX::MaterialLoader::load("./res/materials/" + key);
}
