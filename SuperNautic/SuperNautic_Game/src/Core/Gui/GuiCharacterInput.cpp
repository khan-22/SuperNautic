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
    updateText();
    _text.setOrigin(_text.getLocalBounds().left, _text.getLocalBounds().top);
    _text.setPosition(0.f, 0.f);
    sf::FloatRect textBounds = _text.getLocalBounds();
    _text.setFillColor(sf::Color::White);
    _text.setOutlineColor(sf::Color::Black);
    _text.setOutlineThickness(2.f);

}

void GuiCharacterInput::setOnChange(const std::function<void(char)>& callback)
{
    _onChangeCallback = callback;
}


void GuiCharacterInput::select()
{
    setScale(1.2f, 1.2f);
    _text.setFillColor(sf::Color(200, 200, 90));
}

void GuiCharacterInput::deselect()
{
    setScale(1.0f, 1.0f);
    _text.setFillColor(sf::Color::White);
}

void GuiCharacterInput::handleEventCurrent(const sf::Event& event)
{
    switch(event.key.code)
    {
        case sf::Keyboard::Up:
            _stepSound.play();
            selectPreviousCharacter();
            break;
        case sf::Keyboard::Down:
            _stepSound.play();
            selectNextCharacter();
            break;
        case sf::Keyboard::Z:
            _stepSound.play();
            selectPreviousCharacterList();
            break;
        case sf::Keyboard::X:
            _stepSound.play();
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
    return getWorldTransform().transformRect(_text.getGlobalBounds());
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
        _currentCharacterListIndex = static_cast<unsigned char>(_characterLists.size() - 1);
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
    static constexpr int NUM_LETTERS = 'z' - 'a' + 1;
    static constexpr int NUM_DIGITS = '9' - '0' + 1;
    CharacterFlags list = getCurrentCharacterList();
    switch(list)
    {
        case CharacterFlags::LOWERCASE:
        case CharacterFlags::UPPERCASE:
            if(bForward)
            {
                _currentLetter++;
                _currentLetter %= NUM_LETTERS;
            }
            else
            {
                _currentLetter--;
                if(_currentLetter > NUM_LETTERS)
                {
                    _currentLetter = NUM_LETTERS - 1;
                }
            }
            break;
        case CharacterFlags::DIGITS:
            if(bForward)
            {
                _currentDigit--;
                if(_currentDigit > NUM_DIGITS)
                {
                    _currentDigit = NUM_DIGITS - 1;
                }
            }
            else
            {
                _currentDigit++;
                _currentDigit %= NUM_DIGITS;
            }
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

    bool bHasCharacterChanged = character != _text.getString();
    _text.setString(character);
    if(bHasCharacterChanged)
    {
        _onChangeCallback(character);
    }
}

char GuiCharacterInput::getCharacter() const
{
    assert(_text.getString().getSize() == 1);
    return _text.getString()[0];
}

void GuiCharacterInput::setCharacter(char character)
{
    CharacterFlags flags = CharacterFlags::INVALID;
    unsigned char charIndex = 0;
    if(character >= 'a' && character <= 'z')
    {
        flags = CharacterFlags::LOWERCASE;
        charIndex = character - 'a';
    }
    else if(character >= 'A' && character <= 'Z')
    {
        flags = CharacterFlags::UPPERCASE;
        charIndex = character - 'A';
    }
    else if(character >= '0' && character <= '9')
    {
        flags = CharacterFlags::DIGITS;
        charIndex = character - '0';
    }

    auto found = std::find(_characterLists.begin(), _characterLists.end(), flags);
    if(found != _characterLists.end())
    {
        _currentCharacterListIndex = static_cast<unsigned char>(std::distance(_characterLists.begin(), found));
        if(flags == CharacterFlags::DIGITS)
        {
            _currentDigit = charIndex;
        }
        else
        {
            _currentLetter = charIndex;
        }

        updateText();
        return;
    }

    LOG_ERROR("Invalid character: ", character, " (", (int)character, ")");
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
