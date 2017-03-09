#pragma once

#ifndef CONTROLS_APPLICATION_STATE_HPP
#define CONTROLS_APPLICATION_STATE_HPP

#include "SFML/Graphics/Font.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Core/ApplicationState/ApplicationState.hpp"
#include "Core/Asset/Asset.hpp"
#include "Core/Gui/GuiContainer.hpp"
#include "Core/Gui/GuiTexture.hpp"
#include "Core/Io/Input.hpp"


class ControlsApplicationState : public ApplicationState
{
public:
	ControlsApplicationState(ApplicationStateStack& stack, ApplicationContext& context);

	void render() override;
	bool bUpdate(float dtSeconds) override;
	bool bHandleEvent(const sf::Event& event) override;

private:
	SceneNode _guiGraph;
	FontAsset _font;
	Input _input;
};


#endif //CONTROLS_APPLICATION_STATE_HPP
