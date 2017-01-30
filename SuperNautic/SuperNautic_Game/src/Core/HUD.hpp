#pragma once

#ifndef HUD_HPP
#define HUD_HPP

#include "SceneNode.hpp"

class HUD : public SceneNode
{
public:
	HUD();
	~HUD();

	void setHeat(float heat);
	void setSpeed(float speed);
	void setPosition(int position);

	void updateCurrent() override;
	void renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	float _heat;
	float _speed;
	int _position;

	sf::Font _font;
	sf::Text _tHeat;
	sf::Text _tSpeed;
	sf::Text _tPosition;

};

#endif // !HUD_HPP
