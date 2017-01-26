#include <SFML\Window\Joystick.hpp>

#include "Input.h"

Input::Input()
{
	_controllerId = 0;
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
		_triggers = sf::Joystick::getAxisPosition(_controllerId, sf::Joystick::Z);
	}
}

bool Input::checkActive()
{
	return sf::Joystick::isConnected(_controllerId);
}

bool Input::getA()
{
	return _bButtonA;
}

float Input::getLeftX()
{
	return _leftStickX;
}

float Input::getTriggers()
{
	return _triggers;
}