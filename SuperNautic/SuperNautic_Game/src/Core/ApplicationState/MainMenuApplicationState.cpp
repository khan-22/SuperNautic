#include <iostream>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/Event.hpp"

#include "Core/ApplicationState/MainMenuApplicationState.hpp"
#include "Core/ApplicationState/OptionsApplicationState.hpp"
#include "Core/ApplicationState/ControlsApplicationState.hpp"
#include "Core/ApplicationState/AboutApplicationState.hpp"
#include "Core/ApplicationState/ApplicationStateStack.hpp"
#include "Core/ApplicationState/ApplicationContext.hpp"
#include "Core/ApplicationState/PreGameApplicationState.hpp"
#include "Core/Asset/AssetCache.hpp"
#include "Core/Gui/GuiButton.hpp"
#include "Core/Utility/Camera.h"
#include "GFX/Rendering/SfmlRenderer.hpp"

#include <ecs/ecs.hpp>


#include <functional>

#include "Core/Gui/Font.hpp"
#include "Core/Gui/TextSystem.hpp"
#include "Core/Gui/MenuItemSystem.hpp"
#include "Core/Gui/WorldTransformSystem.hpp"


MainMenuApplicationState::MainMenuApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
: ApplicationState(stack, context)
, _input()
{
    std::cout << "Welcome to MainMenu state." << std::endl;

    MenuElement& root = _menu.get_root();
    sf::Vector2u windowSize = context.window.getSize();
    root.set_position(windowSize.x / 2.f, windowSize.y / 2.f);


    MenuElement& title = root.add_text();
    title.set_position(0.f, -100.f);

    Font font;
    font.font = FontCache::get("res/basictitlefont.ttf");
    font.char_size = 100;
    font.outline_thickness = 0.f;
    font.fill_color = sf::Color::White;
    title.set_font(font);
    title.set_scale(1.5f, 1.f);
    title.set_text("SUPERNAUTIC");


    MenuElement& play = root.add_button();
    play.set_position(0.f, 0.f);
    play.set_text("Play");
    play.set_on_click([&](MenuElement&)
    {
        _stack.pop();
        _stack.push(std::unique_ptr<ApplicationState>(new PreGameApplicationState(_stack, _context)));
    });
    play.select();

    MenuElement& options = root.add_button();
    options.set_position(0.f, 40.f);
    options.set_text("Options");
    options.set_on_click([&](MenuElement&)
    {
        _stack.clear();
        _stack.push(std::unique_ptr<ApplicationState>(new OptionsApplicationState(_stack, _context)));
    });

    MenuElement& controls = root.add_button();
    controls.set_position(0.f, 80.f);
    controls.set_text("Controls");
    controls.set_on_click([&](MenuElement&)
    {
        _stack.clear();
        _stack.push(std::unique_ptr<ApplicationState>(new ControlsApplicationState(_stack, _context)));
    });

    MenuElement& about = root.add_button();
    about.set_position(0.f, 120.f);
    about.set_text("About");
    about.set_on_click([&](MenuElement&)
    {
        _stack.clear();
        _stack.push(std::unique_ptr<ApplicationState>(new AboutApplicationState(_stack, _context)));
    });

    MenuElement& quit = root.add_button();
    quit.set_position(0.f, 160.f);
    quit.set_text("Quit");
    quit.set_on_click([&](MenuElement&)
    {
        _stack.clear();
    });


}

void MainMenuApplicationState::render()
{
    GFX::SfmlRenderer renderer;
    renderer.render(*_context.menuBackground);
    renderer.render(_guiContainer);

    TextSystem::update(renderer);

    renderer.display(_context.window);
//    _context.window.draw(_guiContainer);
}

bool MainMenuApplicationState::bUpdate(float dtSeconds)
{
    WorldTransformSystem::update();

    _context.menuBackground->update(dtSeconds);

	if (_input.checkActive())
	{
		_input.update(dtSeconds);
		for (const sf::Event& e : _input.getEvents())
		{
			_guiContainer.handleEvent(e);
		}
	}

	int playersFound = 0;
	for (int i = 0; i < 5; i++)
	{
		if (sf::Joystick::isConnected(i)) {
			playersFound++;
		}
	}
	_playersActive = playersFound;
	if (_playersActive == 0)
	{
		_playersActive = 1;
	}
    return true;
}

bool MainMenuApplicationState::bHandleEvent(const sf::Event& event)
{
    _guiContainer.handleEvent(event);
 
    MenuItemSystem::handle_event(event);
    
    return true;
}
