#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Window/Event.hpp"

#include "Core/Gui/GuiCharacterInput.hpp"
#include "Core/Asset/AssetCache.hpp"
#include "Core/Utility/Utilities.hpp"

GuiCharacterInput::GuiCharacterInput(CharacterFlags flags)
: _font(AssetCache<sf::Font, std::string>::get("res/arial.ttf"))
, _text("a", *_font.get())
, _characterLists(generateCharacterLists(flags))
{
    sf::FloatRect textBounds = _text.getLocalBounds();
    setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
    _text.setFillColor(sf::Color::Black);
}

void GuiCharacterInput::onChange(const std::function<void(char)>& callback)
{
    _onChangeCallback = callback;
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

void GuiCharacterInput::handleEventCurrent(const sf::Event& event)
{
    switch(event.key.code)
    {
        case sf::Keyboard::Left:
            selectPreviousCharacter();
            break;
        case sf::Keyboard::Right:
            selectNextCharacter();
            break;
        case sf::Keyboard::Z:
            selectPreviousCharacterList();
            break;
        case sf::Keyboard::X:
            selectNextCharacterList();
            break;
        default:
            break;
    }
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
    assert(!_characterLists.empty());
    _currentCharacterListIndex--;
    if(_currentCharacterListIndex > _characterLists.size())
    {
        _currentCharacterListIndex = _characterLists.size() - 1;
    }
    updateText();
}

void GuiCharacterInput::selectNextCharacterList()
{
    assert(!_characterLists.empty());
    _currentCharacterListIndex++;
    _currentCharacterListIndex %= _characterLists.size();
    updateText();
}

GuiCharacterInput::CharacterFlags GuiCharacterInput::getCurrentCharacterList() const
{
    assert(_currentCharacterListIndex < _characterLists.size());
    return _characterLists[_currentCharacterListIndex];
}

void GuiCharacterInput::stepCharacter(bool bForward)
{
    static constexpr int NUM_LETTERS = 'z' - 'a';
    static constexpr int NUM_DIGITS = '9' - '0';
    char direction = bForward ? 1 : -1;
    CharacterFlags list = getCurrentCharacterList();
    switch(list)
    {
        case CharacterFlags::LOWERCASE:
        case CharacterFlags::UPPERCASE:
            _currentLetter = clamp(_currentLetter + direction, 0, NUM_LETTERS);
            break;
        case CharacterFlags::DIGITS:
            _currentDigit = clamp(_currentDigit + direction, 0, NUM_DIGITS);
            break;
        default:
            LOG_ERROR("Unexpected character flag: ", (int)list);
            return;
    }

    updateText();
}

void GuiCharacterInput::updateText()
{
    CharacterFlags list = getCurrentCharacterList();
    char character = ' ';
    switch(list)
    {
        case CharacterFlags::LOWERCASE:
            character = 'a' + _currentLetter;
            break;
        case CharacterFlags::UPPERCASE:
            character = 'A' + _currentLetter;
            break;
        case CharacterFlags::DIGITS:
            character = '0' + _currentDigit;
            break;
        default:
            LOG_ERROR("Unexpected character flag: ", (int)list);
            break;
    }
    if(character != _text.getString())
    {
        _onChangeCallback(character);
    }
    _text.setString(character);
}

std::vector<GuiCharacterInput::CharacterFlags> GuiCharacterInput::generateCharacterLists(CharacterFlags flags)
{
    using F = CharacterFlags;
    std::vector<F> lists;
    if((unsigned char)flags & (unsigned char)F::LOWERCASE)
    {
        lists.push_back(F::LOWERCASE);
    }
    if((unsigned char)flags & (unsigned char)F::UPPERCASE)
    {
        lists.push_back(F::UPPERCASE);
    }
    if((unsigned char)flags & (unsigned char)F::DIGITS)
    {
        lists.push_back(F::DIGITS);
    }

    return lists;
}
