#include <SFML\Window\Joystick.hpp>
#include <SFML\Window\Event.hpp>


#include "Core/Io/Log.hpp"
#include "Core/Io/Input.hpp"

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
	const int thresh = 20;

	if (checkActive())
	{
		_bButtonA = sf::Joystick::isButtonPressed(_controllerId, 0);
		_bButtonY = sf::Joystick::isButtonPressed(_controllerId, 2);
		_bButtonStart = sf::Joystick::isButtonPressed(_controllerId, 7);
		_leftStickX = sf::Joystick::getAxisPosition(_controllerId, sf::Joystick::X);
		_leftStickY = sf::Joystick::getAxisPosition(_controllerId, sf::Joystick::Y);
		_triggers = sf::Joystick::getAxisPosition(_controllerId, sf::Joystick::Z);

		_events.clear();

		if (abs(_leftStickX) < thresh)
		{
			_leftStickX = 0;
		}

		if (abs(_triggers) < 5)
		{
			_triggers = 0;
		}

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
		if (_bButtonY && _bAWasDormant)
		{
			event.type = sf::Event::KeyPressed;
			event.key.code = sf::Keyboard::Y;
			_events.push_back(event);
			_bButtonY = true;
			_bYWasDormant = false;
			LOG("Y pressed");
		}
		if (!_bButtonY)
		{
			_bAWasDormant = true;
		}
		if (_bButtonStart && _bStartWasDormant)
		{
			event.type = sf::Event::KeyPressed;
			event.key.code = sf::Keyboard::Escape;
			_events.push_back(event);
			_bButtonStart = true;
			_bStartWasDormant = false;
			LOG("Start pressed");
		}
		if (!_bButtonStart)
		{
			_bStartWasDormant = true;
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
