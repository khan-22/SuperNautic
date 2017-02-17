#include "SFML/Graphics/RenderTarget.hpp"

#include "Core/Gui/GuiCharacterInput.hpp"
#include "Core/Asset/AssetCache.hpp"
#include "Core/Utility/Utilities.hpp"

GuiCharacterInput::GuiCharacterInput(CharacterFlags flags)
: _font(AssetCache<sf::Font, std::string>::get("res/arial.ttf"))
, _text("a", *_font.get())
{
    sf::FloatRect textBounds = _text.getLocalBounds();
    setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
    _text.setFillColor(sf::Color::Black);
}

void GuiCharacterInput::select()
{
    setScale(1.2f, 1.2f);
    _text.setFillColor(sf::Color::White);
}

void GuiCharacterInput::deselect()
{
    setScale(1.0f, 1.0f);
    _text.setFillColor(sf::Color::Black);
}



void GuiCharacterInput::renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_text, states);
}

bool GuiCharacterInput::bIsSelectable() const
{
    return true;
}

sf::FloatRect GuiCharacterInput::getBoundingRect() const
{
    return getWorldTransform().transformRect(_text.getLocalBounds());
}

void GuiCharacterInput::selectNextCharacter()
{
    stepCharacter(true);
}

void GuiCharacterInput::selectPreviousCharacter()
{
    stepCharacter(false);
}

void GuiCharacterInput::selectPreviousCharacterList()
{

}

void GuiCharacterInput::selectNextCharacterList()
{

}

GuiCharacterInput::CharacterFlags GuiCharacterInput::getCurrentCharacterList() const
{
    return _characterLists[_currentCharacterListIndex];
}

void GuiCharacterInput::stepCharacter(bool bForward)
{
    char direction = bForward ? 1 : -1;
    CharacterFlags list = getCurrentCharacterList();
    switch(list)
    {
    case CharacterFlags::ONLY_LETTERS:
        _currentLetter = clamp(_currentLetter + direction, 0, 10);
        break;
    case CharacterFlags::DIGITS:
        _currentDigit += clamp(_currentDigit + direction, 0, 10);
        break;
    default:
        LOG_ERROR("Unexpected character flag: ", (int)list);
        return;
    }
}
