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
        GuiPlayerJoin(unsigned char playerId);

        sf::FloatRect getBoundingRect() const override;

        void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);


        void setOnJoin(const std::function<void(unsigned char)>& callback);
        void setOnLeave(const std::function<void(unsigned char)>& callback);

    private:
        unsigned char _id;
        sf::Color _color;
        bool _bHasJoined = false;
        Input _input;
        sf::RectangleShape _window;
        std::function<void(unsigned char)> _joinCallback = [](unsigned char){};
        std::function<void(unsigned char)> _leaveCallback = [](unsigned char){};

        virtual void renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual void updateCurrent() override;

        void join();
        void leave();


};

#endif //GUI_PLAYER_JOIN_HPP
