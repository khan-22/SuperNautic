#pragma once

#ifndef GUI_CHARACTER_INPUT_HPP
#define GUI_CHARACTER_INPUT_HPP


#include <functional>

#include "SFML/Graphics/Text.hpp"

#include "Core/Gui/GuiElement.hpp"
#include "Core/Asset/Asset.hpp"

class GuiCharacterInput : public GuiElement
{
    public:
        enum class CharacterFlags : unsigned char
        {
            LOWERCASE   = 1 << 0,
            UPPERCASE   = 1 << 1,
            DIGITS      = 1 << 2,
            ONLY_LETTERS = LOWERCASE | UPPERCASE,
            ALL = ONLY_LETTERS | DIGITS,
        };



        GuiCharacterInput(CharacterFlags flags = CharacterFlags::ALL);

        bool bIsSelectable() const override;
        sf::FloatRect getBoundingRect() const override;

        void selectNextCharacter();
        void selectPreviousCharacter();
        void selectPreviousCharacterList();
        void selectNextCharacterList();

        CharacterFlags getCurrentCharacterList() const;

        void onChange(const std::function<void(char)>& callback);


    private:
        Asset<sf::Font> _font;
        sf::Text _text;
        unsigned char _currentLetter = 0;
        unsigned char _currentDigit = 0;
        std::vector<CharacterFlags> _characterLists;
        unsigned char _currentCharacterListIndex = 0;
        std::function<void(char)> _onChangeCallback;

        void handleEventCurrent(const sf::Event& event) override;
        void renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
        void select() override;
        void deselect() override;
        void stepCharacter(bool bForward);
        void updateText();
        static std::vector<CharacterFlags> generateCharacterLists(CharacterFlags flags);


};

#endif //GUI_CHARACTER_INPUT_HPP
