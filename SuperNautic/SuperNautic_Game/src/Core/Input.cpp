#include <SFML\Window\Joystick.hpp>

#include "Input.hpp"

#include "../Log.hpp"

Input::Input()
{
	_controllerId = 0;
	_bLeftStickDormant = true;
}

Input::Input(int id)
{
	_controllerId = id;
	_bLeftStickDormant = true;
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

	if (_bLeftStickDormant)
	{
		if (_leftStickY > 10)
		{
			_events.push_back(InputEvent::LEFTSTICK_UP);
		}
		else if (_leftStickY < 10)
		{
			_events.push_back(InputEvent::LEFTSTICK_DOWN);
		}
		else if (_leftStickX > 10)
		{
			_events.push_back(InputEvent::LEFTSTICK_RIGHT);
		}
		else if (_leftStickX < 10)
		{
			_events.push_back(InputEvent::LEFTSTICK_LEFT);
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
	return _leftStickX;
}

float Input::getTriggersValue()
{
	return _triggers;
}

std::list<InputEvent> Input::getEvent()
{
	return _events;
}

int Input::getControllerId()
{
	return _controllerId;
}