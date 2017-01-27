// EXAMPLE THING.
// TODO: Talk Karl/Toby through about this.

#include "LoadAsset.hpp"

#include "SFML/Graphics/Font.hpp"

template<>
std::shared_ptr<sf::Font> loadAsset<sf::Font>(std::string key)
{
    auto font = std::make_shared<sf::Font>();
    if(font->loadFromFile(key))
    {
        return font;
    }
    else
    {
        return nullptr;
    }
}
