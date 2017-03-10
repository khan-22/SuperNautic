#include "Core/Gui/GuiTexture.hpp"


GuiTexture::GuiTexture()
{
}

GuiTexture::GuiTexture(const std::string& str)
{
    setTexture(str);
}

void GuiTexture::setTexture(const std::string& str)
{
    _texture = SFMLTextureCache::get(str);

    _sprite.setFillColor(sf::Color::White);
	_sprite.setTexture(_texture.get(), true);
	_sprite.setSize(sf::Vector2f(_texture.get()->getSize().x, _texture.get()->getSize().y));
}

void GuiTexture::setSize(float x, float y)
{
    _sprite.setSize(sf::Vector2f(x, y));
}

void GuiTexture::renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
}

sf::FloatRect GuiTexture::getBoundingRect() const
{
	return getWorldTransform().transformRect(_sprite.getGlobalBounds());
}

void GuiTexture::setFillColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_sprite.setFillColor(sf::Color(r, g, b, a));
}

void GuiTexture::setOutlineThickness(float thickness)
{
	_sprite.setOutlineThickness(thickness);
}

void GuiTexture::setOutlinecolor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_sprite.setOutlineColor(sf::Color(r, g, b, a));
}
