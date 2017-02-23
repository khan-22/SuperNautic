#pragma once

#ifndef GUI_PLAYER_JOIN_CONTAINER_HPP
#define GUI_PLAYER_JOIN_CONTAINER_HPP

#include <functional>

#include "Core/Gui/GuiContainer.hpp"

class GuiPlayerJoin;

class GuiPlayerJoinContainer : public GuiContainer
{
    public:
        GuiPlayerJoinContainer();

        void setOnJoin(const std::function<void(unsigned char)>& callback);
        void setOnLeave(const std::function<void(unsigned char)>& callback);

    private:
        std::vector<GuiPlayerJoin*> _windows;
        std::function<void(unsigned char)> _joinCallback = [](unsigned char){};
        std::function<void(unsigned char)> _leaveCallback = [](unsigned char){};

        void createWindows();
};

#endif //GUI_PLAYER_JOIN_CONTAINER_HPP
