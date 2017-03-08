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
    update();
    _events.clear();
}

Input::Input(int id) :
	_controllerId(id),
	_bLeftStickDormant(true),
	_bAWasDormant(true),
	_bButtonA(false)
{
    update();
    _events.clear();
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
		_bButtonB = sf::Joystick::isButtonPressed(_controllerId, 1);
		_bButtonY = sf::Joystick::isButtonPressed(_controllerId, 3);
		_bButtonL = sf::Joystick::isButtonPressed(_controllerId, 4);
		_bButtonR = sf::Joystick::isButtonPressed(_controllerId, 5);
		_bButtonStart = sf::Joystick::isButtonPressed(_controllerId, 7);
		_leftStickX = sf::Joystick::getAxisPosition(_controllerId, sf::Joystick::Axis::X);
		_leftStickY = sf::Joystick::getAxisPosition(_controllerId, sf::Joystick::Axis::Y);
		_triggers = sf::Joystick::getAxisPosition(_controllerId, sf::Joystick::Axis::Z);
		_dPadX = sf::Joystick::getAxisPosition(_controllerId, sf::Joystick::Axis::PovX);
		_dPadY = sf::Joystick::getAxisPosition(_controllerId, sf::Joystick::Axis::PovY);

		_events.clear();

		if (abs(_leftStickX) < thresh)
		{
			_leftStickX = 0.f;
		}

		if (abs(_triggers) < 5)
		{
			_triggers = 0.f;
		}

		if (abs(_dPadX) < thresh)
		{
			_dPadX = 0.f;
		}

		if (abs(_dPadY) < thresh)
		{
			_dPadY = 0.f;
		}

		sf::Event event;

		if (_bButtonA && _bAWasDormant)
		{
			event.type = sf::Event::KeyPressed;
			event.key.code = sf::Keyboard::A;
			_events.push_back(event);
			_bButtonA = true;
			_bAWasDormant = false;
		}
		if (!_bButtonA)
		{
			_bAWasDormant = true;
		}

		if (_bButtonY && _bYWasDormant)
		{
			event.type = sf::Event::KeyPressed;
			event.key.code = sf::Keyboard::Y;
			_events.push_back(event);
			_bButtonY = true;
			_bYWasDormant = false;
		}
		if (!_bButtonY)
		{
			_bYWasDormant = true;
		}

		if (_bButtonB && _bBWasDormant)
		{
			event.type = sf::Event::KeyPressed;
			event.key.code = sf::Keyboard::B;
			_events.push_back(event);
			_bButtonB = true;
			_bBWasDormant = false;
		}
		if (!_bButtonB)
		{
			_bBWasDormant = true;
		}

		if (_bButtonL && _bLWasDormant)
		{
			event.type = sf::Event::KeyPressed;
			// Micke wanted LB to send Z
			event.key.code = sf::Keyboard::Z;
			_events.push_back(event);
			_bButtonL = true;
			_bLWasDormant = false;
		}
		if (!_bButtonL)
		{
			_bLWasDormant = true;
		}

		if (_bButtonR && _bRWasDormant)
		{
			event.type = sf::Event::KeyPressed;
			// Micke wanted RB to send X
			event.key.code = sf::Keyboard::X;
			_events.push_back(event);
			_bButtonR = true;
			_bRWasDormant = false;
		}
		if (!_bButtonR)
		{
			_bRWasDormant = true;
		}

		if (_bButtonStart && _bStartWasDormant)
		{
			event.type = sf::Event::KeyPressed;
			event.key.code = sf::Keyboard::Escape;
			_events.push_back(event);
			_bButtonStart = true;
			_bStartWasDormant = false;
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
		}
		else if (_leftStickY > thresh && _bLeftStickDormant)
		{
			event.type = sf::Event::KeyPressed;
			event.key.code = sf::Keyboard::Down;
			_events.push_back(event);
			_bLeftStickDormant = false;
		}
		else if (_leftStickX > thresh && _bLeftStickDormant)
		{
			event.type = sf::Event::KeyPressed;
			event.key.code = sf::Keyboard::Right;
			_events.push_back(event);
			_bLeftStickDormant = false;
		}
		else if (_leftStickX < -thresh && _bLeftStickDormant)
		{
			event.type = sf::Event::KeyPressed;
			event.key.code = sf::Keyboard::Left;
			_events.push_back(event);
			_bLeftStickDormant = false;
		}
		else if (_leftStickX > -thresh && _leftStickX < thresh && _leftStickY > -thresh && _leftStickY < thresh)
		{
			_bLeftStickDormant = true;
		}

		if (_dPadX != 0.f && _bDPadXDormant) {
			if (_dPadX > 0)
			{
				event.type = sf::Event::KeyPressed;
				event.key.code = sf::Keyboard::Right;
				_events.push_back(event);
			}
			else
			{
				event.type = sf::Event::KeyPressed;
				event.key.code = sf::Keyboard::Left;
				_events.push_back(event);
			}
			_bDPadXDormant = false;
		}
		else if (_dPadX == 0)
		{
			_bDPadXDormant = true;
		}

		if (_dPadY != 0.f && _bDPadYDormant) {
			if (_dPadY > 0)
			{
				event.type = sf::Event::KeyPressed;
				event.key.code = sf::Keyboard::Up;
				_events.push_back(event);
			}
			else
			{
				event.type = sf::Event::KeyPressed;
				event.key.code = sf::Keyboard::Down;
				_events.push_back(event);
			}
			_bDPadYDormant = false;
		}
		else if (_dPadY == 0)
		{
			_bDPadYDormant = true;
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
	return _leftStickX / 100.f;
}

float Input::getLeftStickYValue()
{
	return _leftStickX / 100.f;
}

float Input::getTriggersValue()
{
	return -_triggers / 100.f;
}

std::list<sf::Event> Input::getEvents()
{
	return _events;
}

int Input::getControllerId()
{
	return _controllerId;
}

void Input::setControllerId(int id)
{
	_controllerId = id;
}
