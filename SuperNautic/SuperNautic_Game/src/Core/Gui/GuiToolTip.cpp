#include "Core/Gui/GuiToolTip.hpp"


GuiToolTip::GuiToolTip(FontAsset font)
: GuiText("", font)
{

}

void GuiToolTip::registerTip(GuiElement* element, std::string tip)
{
    element->registerOnSelect([this, tip]()
    {
        setText(tip);
        center();
    });
    element->registerOnDeselect([this, tip]()
    {
        setText("");
        center();
    });
}

void GuiToolTip::centerAt(size_t x, size_t y)
{
    _centerX = x;
    _centerY = y;
    center();
}

void GuiToolTip::center()
{
    sf::FloatRect bounds = getBoundingRect();
    setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    setPosition(_centerX, _centerY);
}
