#include "SFML/Graphics/RenderTarget.hpp"

#include "Core/Gui/GuiTextInput.hpp"

GuiTextInput::GuiTextInput(size_t numCharacters, GuiCharacterInput::CharacterFlags flags)
{

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

void GuiTextInput::activate()
{

}

void GuiTextInput::renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
}

bool GuiTextInput::bIsSelectable() const
{
    return true;
}

sf::FloatRect GuiTextInput::getBoundingRect() const
{
}
