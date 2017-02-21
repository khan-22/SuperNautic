#pragma once

#ifndef GUI_TEXT_INPUT_HPP
#define GUI_TEXT_INPUT_HPP


#include <functional>

#include "SFML/Graphics/Text.hpp"

#include "Core/Gui/GuiContainer.hpp"
#include "Core/Gui/GuiCharacterInput.hpp"

class GuiTextInput : public GuiContainer
{
    public:
        GuiTextInput(size_t numCharacters, GuiCharacterInput::CharacterFlags flags);

        bool bIsActivatable() const override;
        bool bIsSelectable() const override;

        void setOnChange(const std::function<void(const std::string&)>& callback);


    private:
        std::vector<GuiCharacterInput*> _characters;
        std::string _text;
        std::function<void(const std::string&)> _onChangeCallback = [](const std::string&){};

        void select() override;
        void deselect() override;

        void updateCharacter(size_t index, char character);

};

#endif //GUI_TEXT_INPUT_HPP
