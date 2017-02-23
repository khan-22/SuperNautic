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
        character->setOnChange([i, this](char c)
        {
            updateCharacter(i, c);
        });
        character->setPosition(pos);
        pos.x += character->getBoundingRect().width * 1.2f;

        auto uPtr = std::unique_ptr<GuiElement>(character);
        insert(uPtr);
    }
}

void GuiTextInput::setOnChange(const std::function<void(const std::string&)>& callback)
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

void GuiTextInput::activate()
{
    GuiContainer::select();
}

void GuiTextInput::deactivate()
{
    GuiContainer::deselect();
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

void GuiTextInput::setText(const std::string& str)
{
    size_t size = std::min(str.size(), _characters.size());
    for(size_t i = 0; i < size; i++)
    {
        _text[i] = str[i];
        _characters[i]->setCharacter(str[i]);
    }
}

const std::string& GuiTextInput::getText() const
{
    return _text;
}

void GuiTextInput::handleEventCurrent(const sf::Event& event)
{
    if(!bIsActive())
    {
        return;
    }

    GuiContainer::handleEventCurrent(event);
}
