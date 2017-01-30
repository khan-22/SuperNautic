#pragma once

#ifndef HUD_HPP
#define HUD_HPP

#include <SFML\Graphics\Font.hpp>
#include <SFML\Graphics\Text.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Texture.hpp>

#include "SceneNode.hpp"

#include "Asset.hpp"

class HUD : public SceneNode
{
public:
	HUD(int windowWidth, int windowHeight);
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

	Asset<sf::Font> _font;
	Asset<sf::Texture> _texture;
	sf::Sprite _spriteHeat;
	sf::Text _tSpeed;
	sf::Text _tPosition;

};

#endif // !HUD_HPP
