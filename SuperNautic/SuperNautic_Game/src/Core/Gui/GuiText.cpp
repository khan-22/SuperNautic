#include "Core/Gui/GuiText.hpp"


GuiText::GuiText(const std::string& str, FontAsset font)
: _font(font)
, _text(str, *_font.get())
{
    setText(str);

    _text.setOutlineThickness(2.f);
    _text.setOutlineColor(sf::Color::Black);

    deselect();
}

void GuiText::setText(const std::string& str)
{
    _text.setString(str);
    _text.setOrigin(_text.getLocalBounds().left, _text.getLocalBounds().top);
    _text.setPosition(0.f, 0.f);
}

const std::string& GuiText::getText() const
{
    return _text.getString();
}


void GuiText::renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_text, states);
}

sf::FloatRect GuiText::getBoundingRect() const
{
    return getWorldTransform().transformRect(_text.getGlobalBounds());
}

void GuiText::setFillColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    _text.setFillColor(sf::Color(r, g, b, a));
}

void GuiText::setOutlineThickness(float thickness)
{
    _text.setOutlineThickness(thickness);
}

void GuiText::setOutlinecolor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    _text.setOutlineColor(sf::Color(r, g, b, a));
}

void GuiText::setCharacterSize(unsigned int size)
{
    _text.setCharacterSize(size);
}

void GuiText::select()
{
    _text.setFillColor(sf::Color(200, 200, 90));
}

void GuiText::deselect()
{
    _text.setFillColor(sf::Color::White);
}
