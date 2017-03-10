#pragma once

#ifndef GUI_TEXTURE_HPP
#define GUI_TEXTURE_HPP


#include <functional>

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

#include "Core/Gui/GuiElement.hpp"
#include "Core/Asset/LoadAssetFunctions.hpp"

class GuiTexture : public GuiElement
{
public:
    GuiTexture();
	GuiTexture(const std::string& str);

	void setTexture(const std::string& str);

	virtual sf::FloatRect getBoundingRect() const override;

	void setFillColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
	void setOutlineThickness(float thickness);
	void setOutlinecolor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
    void setSize(float x, float y);

private:
	Asset<sf::Texture> _texture;
	sf::RectangleShape _sprite;

	void renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif //GUI_TEXTURE_HPP
