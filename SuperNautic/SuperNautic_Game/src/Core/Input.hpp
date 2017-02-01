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

protected:
	
private:
	float _leftStickX;
	float _leftStickY;
	float _triggers;
	bool _bButtonA;
	bool _bLeftStickDormant;

	int _controllerId;

	std::list<sf::Event> _events;
};

#endif