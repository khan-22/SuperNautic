#pragma once

#ifndef GUI_TEXT_INPUT_HPP
#define GUI_TEXT_INPUT_HPP


#include <functional>

#include "SFML/Graphics/Text.hpp"

#include "Core/Gui/GuiElement.hpp"
#include "Core/Gui/GuiCharacterInput.hpp"

class GuiTextInput : public GuiElement
{
    public:
        GuiTextInput(size_t numCharacters, GuiCharacterInput::CharacterFlags flags);

        bool bIsActivatable() const override;
        bool bIsSelectable() const override;
        sf::FloatRect getBoundingRect() const override;

    private:
        std::list<GuiCharacterInput> _slots;

        void renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
        void select() override;
        void deselect() override;
        void activate() override;

};

#endif //GUI_TEXT_INPUT_HPP
