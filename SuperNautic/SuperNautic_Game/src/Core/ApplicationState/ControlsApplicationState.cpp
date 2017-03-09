#include <iostream>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"

#include "Core/ApplicationState/ControlsApplicationState.hpp"
#include "Core/ApplicationState/MainMenuApplicationState.hpp"
#include "Core/ApplicationState/ApplicationStateStack.hpp"
#include "Core/ApplicationState/ApplicationContext.hpp"
#include "Core/Asset/AssetCache.hpp"
#include "GFX/Rendering/SfmlRenderer.hpp"


ControlsApplicationState::ControlsApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
	: ApplicationState(stack, context)
	, _font(FontCache::get("res/arial.ttf"))
{
	sf::Vector2u windowSize = _context.window.getSize();

	GuiText* title = new GuiText("Controls", _font);
	title->setCharacterSize(50);
	title->setOrigin(title->getBoundingRect().width / 2.f, title->getBoundingRect().height / 2.f);
	title->setPosition(windowSize.x / 2.f, windowSize.y / 5.f);
	auto titlePtr = std::unique_ptr<SceneNode>(title);
	_guiGraph.attachChild(titlePtr);

	GuiTexture* controls = new GuiTexture("controls.png");
	controls->setScale(windowSize.x / 2.f, windowSize.y / 2.f);
	controls->setOrigin(controls->getBoundingRect().width / 2.f, controls->getBoundingRect().height / 2.f);
	controls->setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
	auto controlsPtr = std::unique_ptr<SceneNode>(controls);
	_guiGraph.attachChild(controlsPtr);
}

void ControlsApplicationState::render()
{
	GFX::SfmlRenderer renderer;
	renderer.render(*_context.menuBackground);
	renderer.render(_guiGraph);
	renderer.display(_context.window);
}

bool ControlsApplicationState::bUpdate(float dtSeconds)
{
	_context.menuBackground->update(dtSeconds);

	if (_input.checkActive())
	{
		_input.update();
		for (const sf::Event& e : _input.getEvents())
		{
			if (e.type == sf::Event::KeyPressed)
			{
				switch (e.key.code)
				{
				case sf::Keyboard::Escape:
				case sf::Keyboard::B:
					_stack.clear();
					_stack.push(std::unique_ptr<ApplicationState>(new MainMenuApplicationState(_stack, _context)));
					return true;

				default:
					break;
				}
			}

			_guiGraph.handleEvent(e);
		}
	}
	return true;
}

bool ControlsApplicationState::bHandleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Escape:
		case sf::Keyboard::B:
			_stack.clear();
			_stack.push(std::unique_ptr<ApplicationState>(new MainMenuApplicationState(_stack, _context)));
			return true;

		default:
			break;
		}
	}

	_guiGraph.handleEvent(event);
	return true;
}
