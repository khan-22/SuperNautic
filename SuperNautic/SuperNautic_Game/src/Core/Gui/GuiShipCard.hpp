#pragma once

#ifndef GUI_SHIP_CARD_HPP
#define GUI_SHIP_CARD_HPP


#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

#include "Core/Gui/GuiElement.hpp"
#include "Core/Asset/LoadAssetFunctions.hpp"

class GuiPlayerJoinContainer;

class GuiShipCard : public GuiElement
{
public:
    GuiShipCard(unsigned int id, const GuiPlayerJoinContainer& players);

    sf::FloatRect getBoundingRect() const override;

    void setMaxSize(float width, float height);


private:
    const unsigned int _id;
    const GuiPlayerJoinContainer& _players;
    bool _bHasJoined = false;
    int _shipId = -1;
    float _maxWidth = 999999.f;
    float _maxHeight = 999999.f;
	Asset<sf::Texture> _texture;
	sf::RectangleShape _card;
	sf::RectangleShape _background;

	void renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    void updateCurrent(float dtSeconds) override;
};

#endif //GUI_SHIP_CARD_HPP
