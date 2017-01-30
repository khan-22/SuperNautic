// EXAMPLE THING.
// TODO: Talk Karl/Toby through about this.

#include "LoadAsset.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include "../GFX/VertexDataImporter.hpp"
#include "../GFX/ShaderLoader.hpp"


struct Mesh
{

};


// Load raw mesh/vertex data
template<>
std::shared_ptr<GFX::RawMeshCollection> loadAsset<GFX::RawMeshCollection>(std::string key)
{
	GFX::VertexDataImporter importer("./res/models/");
    return std::shared_ptr<GFX::RawMeshCollection>(importer.importVertexData(key));
}

// Load shaders
template<>
std::shared_ptr<GFX::Shader> loadAsset<GFX::Shader>(std::string key)
{
	GFX::ShaderLoader loader("./src/GFX/Shaders/");
	return std::shared_ptr<GFX::Shader>(loader.loadShader(key));
}

// Load fonts
template<>
std::shared_ptr<sf::Font> loadAsset<sf::Font>(std::string key)
{
	auto font = std::make_shared<sf::Font>();
	if (font->loadFromFile(key))
	{
		return font;
	}
	else
	{
		return nullptr;
	}
}


// Load WAV
template<>
std::shared_ptr<sf::SoundBuffer> loadAsset<sf::SoundBuffer>(std::string key)
{
	auto buffer = std::make_shared<sf::SoundBuffer>();
	if (buffer->loadFromFile("res/audio/" + key + ".wav"))
	{
		return buffer;
	}
	else
	{
		return nullptr;
	}
}