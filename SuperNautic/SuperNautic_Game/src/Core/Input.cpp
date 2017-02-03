#include <SFML\Window\Joystick.hpp>
#include <SFML\Window\Event.hpp>

#include "Input.hpp"

#include "../Log.hpp"

Input::Input() : 
	_controllerId(0),
	_bLeftStickDormant(true),
	_bAWasDormant(true),
	_bButtonA(false)
{
}

Input::Input(int id) :
	_controllerId(id),
	_bLeftStickDormant(true),
	_bAWasDormant(true),
	_bButtonA(false)
{
}

Input::~Input()
{

}

void Input::update()
{
	const int thresh = 50;

	if (checkActive())
	{
		_bButtonA = sf::Joystick::isButtonPressed(_controllerId, 0);
		_leftStickX = sf::Joystick::getAxisPosition(_controllerId, sf::Joystick::X);
		_leftStickY = sf::Joystick::getAxisPosition(_controllerId, sf::Joystick::Y);
		_triggers = sf::Joystick::getAxisPosition(_controllerId, sf::Joystick::Z);

		_events.clear();

		sf::Event event;
		if (_bButtonA && _bAWasDormant)
		{
			event.type = sf::Event::KeyPressed;
			event.key.code = sf::Keyboard::A;
			_events.push_back(event);
			_bButtonA = true;
			_bAWasDormant = false;
			LOG("A pressed");
		}
		if (!_bButtonA)
		{
			_bAWasDormant = true;
		}
		if (_leftStickY < -thresh && _bLeftStickDormant)
		{
			event.type = sf::Event::KeyPressed;
			event.key.code = sf::Keyboard::Up;
			_events.push_back(event);
			_bLeftStickDormant = false;
			LOG("Stick up ", _leftStickY);
		}
		else if (_leftStickY > thresh && _bLeftStickDormant)
		{
			event.type = sf::Event::KeyPressed;
			event.key.code = sf::Keyboard::Down;
			_events.push_back(event);
			_bLeftStickDormant = false;
			LOG("Stick downs ", _leftStickY);
		}
		else if (_leftStickX > thresh && _bLeftStickDormant)
		{
			event.type = sf::Event::KeyPressed;
			event.key.code = sf::Keyboard::Right;
			_events.push_back(event);
			_bLeftStickDormant = false;
			LOG("Stick right ", _leftStickX);
		}
		else if (_leftStickX < -thresh && _bLeftStickDormant)
		{
			event.type = sf::Event::KeyPressed;
			event.key.code = sf::Keyboard::Left;
			_events.push_back(event);
			_bLeftStickDormant = false;
			LOG("Stick left ", _leftStickX);
		}
		else if (_leftStickX > -thresh && _leftStickX < thresh && _leftStickY > -thresh && _leftStickY < thresh)
		{
			_bLeftStickDormant = true;
		}
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
	return _leftStickX / 100;
}

float Input::getLeftStickYValue()
{
	return _leftStickX / 100;
}

float Input::getTriggersValue()
{
	return -_triggers / 100;
}

std::list<sf::Event> Input::getEvents()
{
	return _events;
}

int Input::getControllerId()
{
	return _controllerId;
}