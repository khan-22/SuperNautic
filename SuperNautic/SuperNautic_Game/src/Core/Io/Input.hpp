#pragma once

#ifndef INPUT_HPP
#define INPUT_HPP

#include <list>

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
	float getLeftStickYValue();
	float getTriggersValue();

	std::list<sf::Event> getEvents();

	int getControllerId();
	void setControllerId(int id);

protected:

private:
	float _leftStickX;
	float _leftStickY;
	float _triggers;

	bool _bButtonA;
	bool _bButtonB;
	bool _bButtonY;
	bool _bButtonL;
	bool _bButtonR;
	bool _bButtonStart;

	bool _bLeftStickDormant;
	bool _bAWasDormant;
	bool _bBWasDormant;
	bool _bYWasDormant;
	bool _bLWasDormant;
	bool _bRWasDormant;
	bool _bStartWasDormant;

	int _controllerId;

	std::list<sf::Event> _events;
};

#endif
