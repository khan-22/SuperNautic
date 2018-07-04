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

struct IsClickable
{
};

struct IsActiveMenuItem
{
};

struct MenuItem
{
    ecs::Entity previous;
    ecs::Entity next;
};

struct Origin
{

};

void make_button(ecs::Entity& button, 
                 sf::Text text, 
                 const std::function<void()>& callback,
                 ecs::Entity previous, 
                 ecs::Entity next)
{
    //text.setOrigin(text.getLocalBounds().left, text.getLocalBounds().top);
    //text.setPosition(0.f, 0.f);
    //sf::FloatRect textBounds = text.getGlobalBounds();
    //setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2.f);

    button += text;
    MenuItem& item = button += MenuItem();
    item.previous = previous;
    item.next = next;

    button += callback;
}


MainMenuApplicationState::MainMenuApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
: ApplicationState(stack, context)
, _font(FontCache::get("res/arial.ttf"))
, _input()
, _titleText("SUPERNAUTIC", FontCache::get("res/basictitlefont.ttf"))
{
    std::cout << "Welcome to MainMenu state." << std::endl;

    _guiContainer.setBackground(sf::Color::Magenta);
    sf::Text text;
    text.setFont(*_font.get());
    text.setFillColor(sf::Color::White);

    text.setString("Play");
    auto play = std::unique_ptr<GuiElement>(new GuiButton(text, [&]()
    {
        _stack.pop();
        _stack.push(std::unique_ptr<ApplicationState>(new PreGameApplicationState(_stack, _context)));
    }));

    text.setString("Options");
    auto options = std::unique_ptr<GuiElement>(new GuiButton(text, [&]()
    {
        _stack.clear();
        _stack.push(std::unique_ptr<ApplicationState>(new OptionsApplicationState(_stack, _context)));
    }));

	text.setString("Controls");
	auto controls = std::unique_ptr<GuiElement>(new GuiButton(text, [&]()
	{
		_stack.clear();
		_stack.push(std::unique_ptr<ApplicationState>(new ControlsApplicationState(_stack, _context)));
	}));

    
    // auto about = std::unique_ptr<GuiElement>(new GuiButton(text, [&]()
    // {
    //     _stack.clear();
    //     _stack.push(std::unique_ptr<ApplicationState>(new AboutApplicationState(_stack, _context)));
    // }));

    text.setString("Quit");
    auto quit = std::unique_ptr<GuiElement>(new GuiButton(text, [&]()
    {
        _stack.clear();
    }));



    options->move(0.f, play->getBoundingRect().height * 1.5f);
	controls->setPosition(options->getPosition());
	controls->move(0.f, options->getBoundingRect().height * 1.5f);

    ecs::Entity about = ecs::create_entity();
    about += std::function<void()>([&]()
    {
        _stack.clear();
        _stack.push(std::unique_ptr<ApplicationState>(new AboutApplicationState(_stack, _context)));
    });

    text.setString("About");
    text.setPosition(controls->getPosition());
    text.move(0.f, controls->getBoundingRect().height * 1.5f);
    


	//about->setPosition(controls->getPosition());
	//about->move(0.f, controls->getBoundingRect().height * 1.5f);
    quit->setPosition(text.getPosition());
    quit->move(0.f, text.getGlobalBounds().height * 1.5f);
    _guiContainer.insert(play);
    _guiContainer.insert(options);
	_guiContainer.insert(controls);
    //_guiContainer.insert(about);
    _guiContainer.insert(quit);

    sf::Vector2u windowSize = _context.window.getSize();
    sf::FloatRect guiBounds = _guiContainer.getBoundingRect();
    _guiContainer.setOrigin(guiBounds.left + guiBounds.width / 2.f, guiBounds.top + guiBounds.height / 2.f);
    _guiContainer.setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
    _guiContainer.toggleSelection();
    _guiContainer.setBackground(sf::Color(27, 173, 222, 100), sf::Color(19, 121, 156, 100), 5.f);

    text.move(_guiContainer.getWorldPosition());
    about += text;
    about += text.getPosition();
    about += text.getGlobalBounds();
    about += IsClickable();

    _titleText.setCharacterSize(100);
    _titleText.setOrigin(_titleText.getBoundingRect().width / 2.f, _titleText.getBoundingRect().height / 2.f);
    _titleText.setPosition(windowSize.x / 2.f, windowSize.y / 4.f);
    _titleText.setScale(1.5f, 1.f);
    _titleText.setOutlineThickness(0.f);



    ecs::Entity button_a = ecs::create_entity();
    ecs::Entity button_b = ecs::create_entity();

    text.setString("A");
    text.setPosition(100.f, 100.f);
    make_button(button_a, 
                text, 
                std::function<void()>([](){std::cout << "A" << std::endl;}),
                button_b,
                button_b);


    text.setString("B");
    text.move(0.f, 100.f);
    make_button(button_b, 
                text, 
                std::function<void()>([](){std::cout << "B" << std::endl;}),
                button_a,
                button_a);

    button_a += IsActiveMenuItem();
}

void MainMenuApplicationState::render()
{
    GFX::SfmlRenderer renderer;
    renderer.render(*_context.menuBackground);
    renderer.render(_guiContainer);
    renderer.render(_titleText);

    for(sf::Text* text : ecs::get_components_with<sf::Text>())
    {
        renderer.render(*text);
    }

    renderer.display(_context.window);
//    _context.window.draw(_guiContainer);
}

bool MainMenuApplicationState::bUpdate(float dtSeconds)
{
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
    
    if(event.type == sf::Event::EventType::MouseButtonPressed &&
       event.mouseButton.button == sf::Mouse::Button::Left)
    {
        float x = event.mouseButton.x;
        float y = event.mouseButton.y;
        for(ecs::Entity button : ecs::get_entities_with<std::function<void()>, IsClickable, sf::FloatRect>())
        {
            if(button.get<sf::FloatRect>()->contains(x, y))
            {
                (*button.get<std::function<void()>>())();
                break;
            }
        }
    }

    if(event.type == sf::Event::EventType::KeyPressed)
    {
        switch(event.key.code)
        {
            case sf::Keyboard::Down:
            {
                ecs::Entity current = ecs::get_entity_with<IsActiveMenuItem, MenuItem>();
                if(!current)
                {
                    break;
                }
                ecs::Entity& next = current.get<MenuItem>()->next;
                if(!next)
                {
                    break;
                }

                current.detach<IsActiveMenuItem>();
                next.attach<IsActiveMenuItem>();

                sf::Text* current_text = current;
                sf::Text* next_text = next;

                if(current_text)
                {
                    current_text->setScale(1.f, 1.f);
                }

                if(next_text)
                {
                    next_text->setScale(1.2f, 1.2f);
                }
                
                break;
            }
            
            case sf::Keyboard::Up:
            {   
                ecs::Entity current = ecs::get_entity_with<IsActiveMenuItem, MenuItem>();
                if(!current)
                {
                    break;
                }
                ecs::Entity& previous = current.get<MenuItem>()->previous;
                if(!previous)
                {
                    break;
                }

                current.detach<IsActiveMenuItem>();
                previous.attach<IsActiveMenuItem>();
                

                sf::Text* current_text = current;
                sf::Text* previous_text = previous;

                if(current_text)
                {
                    current_text->setScale(1.f, 1.f);
                }

                if(previous_text)
                {
                    previous_text->setScale(1.2f, 1.2f);
                }

                break;
            }
            case sf::Keyboard::Space:
            {
                std::function<void()>* callback = ecs::get_component_with<std::function<void()>, IsActiveMenuItem>();
                if(callback)
                {
                    (*callback)();
                }
                break;
            }

            default:
                break;


        }
    }
    
    
    return true;
}
