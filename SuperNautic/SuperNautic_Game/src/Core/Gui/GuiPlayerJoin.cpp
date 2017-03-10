#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Window/Event.hpp"

#include "Core/Gui/GuiPlayerJoin.hpp"

GuiPlayerJoin::GuiPlayerJoin()
: _id(-1),
_shipNumber(0)
{
    leave();

    _window.setSize(sf::Vector2f(50.f, 50.f));
    _window.setOutlineThickness(5.f);

	_ship.setSize(sf::Vector2f(50.f, 25.f));

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
	if (_bHasJoined)
	{
		target.draw(_ship, states);
	}
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

unsigned int GuiPlayerJoin::getShipId() const
{
	return _shipNumber;
}

//float GuiPlayerJoin::getMaxAcceleration() const
//{
//	return 100.f;
//}
//
//float GuiPlayerJoin::getMaxTurningSpeed() const
//{
//	return 20.f;
//}
//
//float GuiPlayerJoin::getCooldownOnObstacleCollision() const
//{
//	return 2.f;
//}
//
//float GuiPlayerJoin::getOverheatTemperature() const
//{
//	return .15f;
//}
//
//float GuiPlayerJoin::getOverheatCooldown() const
//{
//	return 4.f;
//}

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

int GuiPlayerJoin::changeShip() 
{
	_shipNumber = (_shipNumber + 1) % 4;
	if (_shipNumber == 0)
	{
		_ship.setSize(sf::Vector2f(50.f, 25.f));
	}
	else if (_shipNumber == 1)
	{
		_ship.setSize(sf::Vector2f(37.5f, 25.f));
	}
	else if (_shipNumber == 2)
	{
		_ship.setSize(sf::Vector2f(25.f, 25.f));
	}
	else
	{
		_ship.setSize(sf::Vector2f(12.5f, 25.f));
	}

	return _shipNumber;
}