#pragma once

#ifndef INPUT_H
#define INPUT_H

class Input
{
public:
	Input();
	~Input();

	void update();
	bool checkActive();

	bool getA();
	float getLeftX();
	float getTriggers();

protected:
	
private:
	float _leftStickX;
	float _triggers;
	bool _bButtonA;

	int _controllerId;
};

#endif