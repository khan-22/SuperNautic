#include "Core/Gui/GuiText.hpp"


GuiText::GuiText(const std::string& str, FontAsset font)
: _font(font)
, _text(str, *_font.get())
{
    _text.setOrigin(_text.getLocalBounds().left, _text.getLocalBounds().top);
    _text.setPosition(0.f, 0.f);
}

void GuiText::setText(const std::string& str)
{
    _text.setString(str);
}

const std::string& GuiText::getText() const
{
    return _text.getString();
}
