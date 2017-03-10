#pragma once

#ifndef GUI_PLAYER_JOIN_HPP
#define GUI_PLAYER_JOIN_HPP

#include <functional>

#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

#include "Core/Gui/GuiElement.hpp"
#include "Core/Io/Input.hpp"


class GuiPlayerJoin : public GuiElement
{
    public:
        GuiPlayerJoin();

        sf::FloatRect getBoundingRect() const override;

        void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

        bool bIsAssigned() const;
        bool bHasJoined() const;

        unsigned int getId() const;
		unsigned int getShipId() const;

        void assign(unsigned int playerId);
        void unassign();

        void toggleJoin();

		int changeShip();

    private:
        unsigned char _id;
        sf::Color _color;
        bool _bHasJoined = false;
        bool _bIsAssigned = false;
        sf::RectangleShape _window;

		unsigned int _shipNumber;

        virtual void renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

        void join();
        void leave();
};

#endif //GUI_PLAYER_JOIN_HPP
