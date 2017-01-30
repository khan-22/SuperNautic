#include <SFML\Window\Joystick.hpp>
#include <SFML\Window\Event.hpp>

#include "Input.hpp"

#include "../Log.hpp"

Input::Input() : 
	_controllerId(0),
	_bLeftStickDormant(true)
{
}

Input::Input(int id) :
	_controllerId(id),
	_bLeftStickDormant(true)
{
}

Input::~Input()
{

}

void Input::update()
{
	if (checkActive())
	{
		_bButtonA = sf::Joystick::isButtonPressed(_controllerId, 0);
		_leftStickX = sf::Joystick::getAxisPosition(_controllerId, sf::Joystick::X);
		_leftStickY = sf::Joystick::getAxisPosition(_controllerId, sf::Joystick::Y);
		_triggers = sf::Joystick::getAxisPosition(_controllerId, sf::Joystick::Z);
	}

	_events.clear();

	sf::Event event;
	if (_leftStickY < -50 && _bLeftStickDormant)
	{
		event.type = sf::Event::KeyPressed;
		event.key.code = sf::Keyboard::Up;
		_events.push_back(event);
		_bLeftStickDormant = false;
		LOG("Stick up", _leftStickY);
	}
	else if (_leftStickY > 50 && _bLeftStickDormant)
	{
		event.type = sf::Event::KeyPressed;
		event.key.code = sf::Keyboard::Down;
		_events.push_back(event);
		_bLeftStickDormant = false;
		LOG("Stick downs ", _leftStickY);
	}
	else if (_leftStickX > 50 && _bLeftStickDormant)
	{
		event.type = sf::Event::KeyPressed;
		event.key.code = sf::Keyboard::Right;
		_events.push_back(event);
		_bLeftStickDormant = false;
		LOG("Stick right", _leftStickX);
	}
	else if (_leftStickX < 50 && _bLeftStickDormant)
	{
		event.type = sf::Event::KeyPressed;
		event.key.code = sf::Keyboard::Left;
		_events.push_back(event);
		_bLeftStickDormant = false;
		LOG("Stick left", _leftStickX);
	}
	else
	{
		_bLeftStickDormant = true;
	}
}

bool Input::checkActive()
{
	return sf::Joystick::isConnected(_controllerId);
}

bool Input::bGetAValue()
{
	return _bButtonA;
}

float Input::getLeftStickXValue()
{
	return _leftStickX;
}

float Input::getTriggersValue()
{
	return _triggers;
}

std::list<sf::Event> Input::getEvents()
{
	return _events;
}

int Input::getControllerId()
{
	return _controllerId;
}