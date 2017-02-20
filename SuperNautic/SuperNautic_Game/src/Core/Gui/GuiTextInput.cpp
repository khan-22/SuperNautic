#include "SFML/Graphics/RenderTarget.hpp"

#include "Core/Gui/GuiTextInput.hpp"

GuiTextInput::GuiTextInput(size_t numCharacters, GuiCharacterInput::CharacterFlags flags)
: GuiContainer(sf::Keyboard::Right, sf::Keyboard::Left)
{
    sf::Vector2f pos;
    for(size_t i = 0; i < numCharacters; i++)
    {
        GuiCharacterInput* character = new GuiCharacterInput(flags);
        _characters.push_back(character);
        _text.push_back(character->getCharacter());
        character->onChange([i, this](char c)
        {
            updateCharacter(i, c);
        });
        character->setPosition(pos);
        pos.x += character->getBoundingRect().width;

        auto uPtr = std::unique_ptr<GuiElement>(character);
        insert(uPtr);
    }
}

void GuiTextInput::onChange(const std::function<void(const std::string&)>& callback)
{
    _onChangeCallback = callback;
}

void GuiTextInput::select()
{
    setScale(1.2f, 1.2f);
}

void GuiTextInput::deselect()
{
    setScale(1.0f, 1.0f);
}

bool GuiTextInput::bIsActivatable() const
{
    return true;
}

bool GuiTextInput::bIsSelectable() const
{
    return true;
}

void GuiTextInput::updateCharacter(size_t index, char character)
{
    assert(index < _characters.size());
    assert(index < _text.size());

    bool bHasTextChanged = _text[index] != character;
    _text[index] = character;
    if(bHasTextChanged)
    {
        _onChangeCallback(_text);
    }
}
