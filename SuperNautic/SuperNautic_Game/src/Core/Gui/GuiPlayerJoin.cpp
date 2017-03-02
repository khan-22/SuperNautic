#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Window/Event.hpp"

#include "Core/Gui/GuiPlayerJoin.hpp"

GuiPlayerJoin::GuiPlayerJoin()
: _id(-1)
{
    leave();

    _window.setSize(sf::Vector2f(50.f, 50.f));
    _window.setOutlineThickness(5.f);

    sf::Color defaultColor = sf::Color::Magenta;
    setColor(defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a);
}

void GuiPlayerJoin::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    _color = sf::Color(r, g, b, a);
    _window.setOutlineColor(_color);
    if(_bHasJoined)
    {
        _window.setFillColor(_color);
    }
}

void GuiPlayerJoin::renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_window, states);
}

sf::FloatRect GuiPlayerJoin::getBoundingRect() const
{
    return getWorldTransform().transformRect(_window.getGlobalBounds());
}


void GuiPlayerJoin::join()
{
    _window.setFillColor(_color);
    _bHasJoined = true;
}

void GuiPlayerJoin::leave()
{
    _window.setFillColor(sf::Color::Transparent);
    _bHasJoined = false;
}


bool GuiPlayerJoin::bIsAssigned() const
{
    return _bIsAssigned;
}

bool GuiPlayerJoin::bHasJoined() const
{
    return _bHasJoined;
}

unsigned int GuiPlayerJoin::getId() const
{
    return _id;
}

void GuiPlayerJoin::assign(unsigned int playerId)
{
    _bIsAssigned = true;
    _bHasJoined = false;
    _id = playerId;
}

void GuiPlayerJoin::unassign()
{
    _bIsAssigned = false;
    _id = -1;
    leave();
}

void GuiPlayerJoin::toggleJoin()
{
    if(_bHasJoined)
    {
        leave();
    }
    else
    {
        join();
    }
}
