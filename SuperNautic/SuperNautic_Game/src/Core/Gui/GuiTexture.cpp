#include "Core/Gui/GuiTexture.hpp"


GuiTexture::GuiTexture(const std::string& str) :
	_texture(SFMLTextureCache::get(str))
{
    setTexture("");
	deselect();
}

void GuiTexture::setTexture(const std::string& str)
{
    _sprite.setFillColor(sf::Color::White);
	_sprite.setTexture(_texture.get(), true);
//	_sprite.setOrigin(_rect.getLocalBounds().left, _rect.getLocalBounds().top);
	_sprite.setPosition(0.f, 0.f);
	_sprite.setSize(sf::Vector2f(100.f, 100.f));
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
//	_rect.setFillColor(sf::Color(r, g, b, a));
}

void GuiTexture::setOutlineThickness(float thickness)
{
//	_rect.setOutlineThickness(thickness);
}

void GuiTexture::setOutlinecolor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
//	_rect.setOutlineColor(sf::Color(r, g, b, a));
}

void GuiTexture::select()
{
//	_rect.setFillColor(sf::Color(200, 200, 90));
}

void GuiTexture::deselect()
{
//	_rect.setFillColor(sf::Color::White);
}
