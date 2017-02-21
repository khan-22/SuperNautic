#include "SFML/Graphics/RenderTarget.hpp"

#include "Core/Gui/GuiButton.hpp"

GuiButton::GuiButton(const sf::Text& text, std::function<void()> callback)
: _text(text)
, _callback(callback)
{
    _text.setOrigin(_text.getLocalBounds().left, _text.getLocalBounds().top);
    _text.setPosition(0.f, 0.f);
    sf::FloatRect textBounds = _text.getGlobalBounds();
    setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
    _text.setFillColor(sf::Color::Black);
}

void GuiButton::select()
{
    setScale(1.2f, 1.2f);
    _text.setFillColor(sf::Color::White);
}

void GuiButton::deselect()
{
    setScale(1.0f, 1.0f);
    _text.setFillColor(sf::Color::Black);
}

bool GuiButton::bIsActivatable() const
{
    return true;
}

void GuiButton::activate()
{
    _callback();
    toggleActivation();
}

void GuiButton::renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_text, states);
}

bool GuiButton::bIsSelectable() const
{
    return true;
}

sf::FloatRect GuiButton::getBoundingRect() const
{
    return getWorldTransform().transformRect(_text.getGlobalBounds());
}
