#pragma once

#ifndef GUI_PLAYER_JOIN_CONTAINER_HPP
#define GUI_PLAYER_JOIN_CONTAINER_HPP

#include <memory>

#include "Core/Gui/GuiContainer.hpp"

class GuiPlayerJoin;

class GuiPlayerJoinContainer : public GuiContainer
{
    public:
        struct Player
        {
            unsigned char color[4]; // rgba
            unsigned int id;
        };

        GuiPlayerJoinContainer();

        std::vector<Player> getJoinedPlayers() const;

    private:
        static constexpr size_t _MAX_PLAYERS = 4;
        static const unsigned char _COLORS[4][_MAX_PLAYERS];

        std::vector<GuiPlayerJoin*> _windows;

        void createWindows();
        void toggle(unsigned int id);
        void drop(unsigned int id);

        virtual void handleEventCurrent(const sf::Event& event) override;
};

#endif //GUI_PLAYER_JOIN_CONTAINER_HPP
