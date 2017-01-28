#pragma once

#ifndef INPUT_HPP
#define INPUT_HPP

#include <list>

enum class InputEvent : unsigned char
{
	A_PRESSED,
	LEFTSTICK_LEFT,
	LEFTSTICK_UP,
	LEFTSTICK_DOWN,
	LEFTSTICK_RIGHT
};

class Input
{
public:
	Input();
	Input(int id);
	~Input();

	void update();
	bool checkActive();

	bool bGetAValue();
	float getLeftStickXValue();
	float getTriggersValue();

	std::list<InputEvent> getEvent();

	int getControllerId();

protected:
	
private:
	float _leftStickX;
	float _leftStickY;
	float _triggers;
	bool _bButtonA;
	bool _bLeftStickDormant;

	int _controllerId;

	std::list<InputEvent> _events;
};

#endif