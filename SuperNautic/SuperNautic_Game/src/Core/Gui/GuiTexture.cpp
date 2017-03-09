#include "Core/Gui/GuiTexture.hpp"


GuiTexture::GuiTexture(const std::string& str) :
	_texture(SFMLTextureCache::get(str))
{
	deselect();
}

void GuiTexture::setTexture(const std::string& str)
{
	_rect.setTexture(_texture.get());
	_rect.setOrigin(_rect.getLocalBounds().left, _rect.getLocalBounds().top);
	_rect.setPosition(0.f, 0.f);
}


void GuiTexture::renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_rect, states);
}

sf::FloatRect GuiTexture::getBoundingRect() const
{
	return getWorldTransform().transformRect(_rect.getGlobalBounds());
}

void GuiTexture::setFillColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_rect.setFillColor(sf::Color(r, g, b, a));
}

void GuiTexture::setOutlineThickness(float thickness)
{
	_rect.setOutlineThickness(thickness);
}

void GuiTexture::setOutlinecolor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_rect.setOutlineColor(sf::Color(r, g, b, a));
}

void GuiTexture::select()
{
	_rect.setFillColor(sf::Color(200, 200, 90));
}

void GuiTexture::deselect()
{
	_rect.setFillColor(sf::Color::White);
}
