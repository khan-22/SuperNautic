#include <iostream>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/Event.hpp"

#include "Core/ApplicationState/PreGameApplicationState.hpp"
#include "Core/ApplicationState/MainMenuApplicationState.hpp"
#include "Core/ApplicationState/ApplicationStateStack.hpp"
#include "Core/ApplicationState/ApplicationContext.hpp"
#include "Core/ApplicationState/PlayApplicationState.hpp"
#include "Core/Asset/AssetCache.hpp"
#include "Core/Gui/GuiButton.hpp"
#include "Core/Utility/Camera.h"
#include "GFX/Rendering/SfmlRenderer.hpp"


#include "Core/Gui/GuiTextInput.hpp"
#include "Core/Gui/GuiHorizontalList.hpp"
#include "Core/Gui/GuiSlider.hpp"
#include "Core/Gui/GuiPlayerJoinContainer.hpp"

PreGameApplicationState::PreGameApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
: ApplicationState(stack, context)
, _trackGenerator(context.segmentHandler.get(), context.obstacleHandler.get())
, _font(AssetCache<sf::Font, std::string>::get("res/arial.ttf"))
, _input()
, _numPlayers(0)
, _guiContainer(_trackGenerator, _context.track.get())
{
    std::vector<std::unique_ptr<GuiElement>> guiElements;

    GuiButton* startButton = new GuiButton(sf::Text("Start", *_font.get()), [&]()
    {
        _stack.clear();
        _context.numPlayers = _numPlayers == 0 ? 1 : _numPlayers;
        _context.track = _trackGenerator.takeTrack();
        _stack.push(std::unique_ptr<ApplicationState>(new PlayApplicationState(_stack, _context)));
    });
    guiElements.emplace_back(startButton);


    GuiButton* backButton = new GuiButton(sf::Text("Back", *_font.get()), [&]()
    {
        _stack.clear();
        _context.track = _trackGenerator.takeTrack();
        _stack.push(std::unique_ptr<ApplicationState>(new MainMenuApplicationState(_stack, _context)));
    });
    guiElements.emplace_back(backButton);

    GuiPlayerJoinContainer* players = new GuiPlayerJoinContainer();
    players->setOnJoin([this](unsigned char playerId)
    {
        _numPlayers++;
    });
    players->setOnLeave([this](unsigned char playerId)
    {
        _numPlayers--;
    });
    guiElements.emplace_back(players);

    sf::Vector2f pos(0.f, _guiContainer.getBoundingRect().top + _guiContainer.getBoundingRect().height);
    for(const std::unique_ptr<GuiElement>& e : guiElements)
    {
        pos.y += e->getBoundingRect().height * 1.5f;
        e->setPosition(pos);
    }

    _guiContainer.setBackground(sf::Color(27, 173, 222, 100), sf::Color(19, 121, 156, 100), 5.f);
    _guiContainer.insert(guiElements);

    sf::Vector2u windowSize = _context.window.getSize();
    sf::FloatRect guiBounds = _guiContainer.getBoundingRect();
    _guiContainer.setOrigin(guiBounds.left + guiBounds.width / 2.f, guiBounds.top + guiBounds.height / 2.f);
    _guiContainer.setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
    _guiContainer.select(startButton);

    _forwardRenderer.initialize(&_context.window, 0.f, 0.f, 1.f, 1.f);
}

void PreGameApplicationState::render()
{
    _forwardRenderer.render(_trackGenerator);
    static Camera garbageCam(glm::radians(90.f), 1280.f, 720.f);
    _forwardRenderer.display(garbageCam);

    _sfmlRenderer.render(_guiContainer);
    _sfmlRenderer.display(_context.window);
}

bool PreGameApplicationState::bUpdate(float dtSeconds)
{
    _guiContainer.update();
    _trackGenerator.update(dtSeconds);

    if(_input.checkActive())
    {
        _input.update();
        for(const sf::Event& e : _input.getEvents())
        {
            if(e.type == sf::Event::KeyPressed)
            {
                switch(e.key.code)
                {
                case sf::Keyboard::B:
                case sf::Keyboard::Escape:
                    if(!_guiContainer.bIsActive())
                    {
                        _stack.clear();
                        _context.track = _trackGenerator.takeTrack();
                        _stack.push(std::unique_ptr<ApplicationState>(new MainMenuApplicationState(_stack, _context)));
                    }
                    break;

                default:
                    break;
                }
            }
            _guiContainer.handleEvent(e);
        }
    }

    return true;
}

bool PreGameApplicationState::bHandleEvent(const sf::Event& event)
{
    if(event.type == sf::Event::KeyPressed)
    {
        switch(event.key.code)
        {
        case sf::Keyboard::B:
        case sf::Keyboard::Escape:
            if(!_guiContainer.bIsActive())
            {
                _stack.clear();
                _context.track = _trackGenerator.takeTrack();
                _stack.push(std::unique_ptr<ApplicationState>(new MainMenuApplicationState(_stack, _context)));
            }
            break;

        default:
            break;
        }
    }
    _guiContainer.handleEvent(event);
    return true;
}
