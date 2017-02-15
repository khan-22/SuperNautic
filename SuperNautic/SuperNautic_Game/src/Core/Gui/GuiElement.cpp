#include "Core/Gui/GuiElement.hpp"

GuiElement::GuiElement()
: SceneNode()
, _bIsSelected(false)
, _bIsActive(false)
{

}

GuiElement::~GuiElement()
{
}

bool GuiElement::bIsSelectable() const
{
    return false;
}

bool GuiElement::bIsSelected() const
{
    return _bIsSelected;
}

void GuiElement::select()
{
    _bIsSelected = true;
}

void GuiElement::deselect()
{
    _bIsSelected = false;
}

void GuiElement::toggleSelection()
{
    _bIsSelected = !_bIsSelected;

    if(_bIsSelected)
    {
        select();
    }
    else
    {
        deselect();
    }

}

void GuiElement::activate()
{
}


void GuiElement::deactivate()
{
}

bool GuiElement::bIsActivatable() const
{
    return false;
}

bool GuiElement::bIsActive() const
{
    return _bIsActive;
}

void GuiElement::toggleActivation()
{
    _bIsActive = !_bIsActive;

    if(_bIsActive)
    {
        activate();
    }
    else
    {
        deactivate();
    }

}
