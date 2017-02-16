#pragma once

#ifndef TIMER_HPP
#define TIMER_HPP

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Core/Gui/SceneNode.hpp"
#include "Core/Asset/Asset.hpp"

class Timer : public SceneNode
{
public:
	Timer(int windowWidth, int windowHeight);
	~Timer();

	void updateTime(float dt);

	void updateCurrent() override;
	void renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	float				_time;

	Asset<sf::Font>		_font;

	sf::Text			_tTime;
};

#endif // !TIMER_HPP