#pragma once

#ifndef GUI_TOOL_TIP_HPP
#define GUI_TOOL_TIP_HPP


#include "Core/Gui/GuiText.hpp"

class GuiToolTip : public GuiText
{
    public:
        GuiToolTip(FontAsset font);

        void centerAt(size_t x, size_t y);
        void registerTip(GuiElement* element, std::string tip);

    private:
        size_t _centerX;
        size_t _centerY;

        void center();
};

#endif //GUI_TOOL_TIP_HPP
