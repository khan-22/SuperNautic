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

#include "Core/Gui/GuiPlayerJoinContainer.hpp"
#include "Core/Gui/GuiHorizontalList.hpp"

PreGameApplicationState::PreGameApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
: ApplicationState(stack, context)
, _font(AssetCache<sf::Font, std::string>::get("res/arial.ttf"))
, _toolTip(_font)
, _trackGenerator(context.segmentHandler.get(), context.obstacleHandler.get())
, _input()
, _guiContainer(_trackGenerator, &_toolTip, _context.track.get())
, _title("Lobby", _font)
{
    std::vector<std::unique_ptr<GuiElement>> guiElements;

    GuiButton* startButton = new GuiButton(sf::Text("Start", *_font.get()), [&]()
    {
        _stack.clear();
        _context.players = _players->getJoinedPlayers();
        _context.track = _trackGenerator.takeTrack();
        _stack.push(std::unique_ptr<ApplicationState>(new PlayApplicationState(_stack, _context)));
    });
    guiElements.emplace_back(startButton);

    GuiHorizontalList* songList = new GuiHorizontalList();
    for(std::string song : _context.audio.getPlaylist())
    {
        GuiText* songText = new GuiText(song, _font);
        songText->registerOnSelect([&context, song]()
        {
            context.audio.changeSong(song);
        });
        auto songTextPtr = std::unique_ptr<GuiElement>(songText);
        songList->insert(songTextPtr);
    }
    guiElements.emplace_back(songList);


    GuiButton* backButton = new GuiButton(sf::Text("Back", *_font.get()), [&]()
    {
        _stack.clear();
        _context.track = _trackGenerator.takeTrack();
        _stack.push(std::unique_ptr<ApplicationState>(new MainMenuApplicationState(_stack, _context)));
    });
    guiElements.emplace_back(backButton);

    _players = new GuiPlayerJoinContainer();
    guiElements.emplace_back(_players);

    sf::Vector2f pos(0.f, _guiContainer.getBoundingRect().top + _guiContainer.getBoundingRect().height);
    for(const std::unique_ptr<GuiElement>& e : guiElements)
    {
        pos.y += e->getBoundingRect().height * 1.75f;
        e->setPosition(pos);
    }

    _guiContainer.setBackground(sf::Color(27, 173, 222, 100), sf::Color(19, 121, 156, 100), 5.f);
    _guiContainer.insert(guiElements);



    sf::Vector2u windowSize = _context.window.getSize();
    sf::FloatRect guiBounds = _guiContainer.getBoundingRect();
    _guiContainer.setOrigin(guiBounds.left + guiBounds.width / 2.f, guiBounds.top + guiBounds.height);
    _guiContainer.setPosition(windowSize.x / 2.f, windowSize.y * 0.9f);

    _toolTip.centerAt(static_cast<size_t>(windowSize.x / 2.f), static_cast<size_t>(windowSize.y * 0.95f));
    _toolTip.registerTip(startButton, "Start the game.");
    _toolTip.registerTip(backButton, "Go back to main menu.");
	_toolTip.registerTip(songList, "Select background music track.");
    _toolTip.setCharacterSize(18);


    _title.setCharacterSize(50);
    _title.setOrigin(_title.getBoundingRect().width / 2.f, _title.getBoundingRect().height / 2.f);
    _title.setPosition(windowSize.x / 2.f, _guiContainer.getBoundingRect().top / 2.f);

    _guiContainer.select(startButton);

    _forwardRenderer.initialize(&_context.window, 0.f, 0.f, 1.f, 1.f, &GFX::Framebuffer::DEFAULT);


}

void PreGameApplicationState::render()
{
    _sfmlRenderer.render(*_context.menuBackground);
    _sfmlRenderer.display(_context.window);

    _forwardRenderer.render(_trackGenerator);
    static Camera garbageCam(glm::radians(90.f), 1280, 720);
    _forwardRenderer.display(garbageCam);

    _sfmlRenderer.render(_guiContainer);
    _sfmlRenderer.render(_toolTip);
    _sfmlRenderer.render(_title);
    _sfmlRenderer.display(_context.window);
}

bool PreGameApplicationState::bUpdate(float dtSeconds)
{
    _context.menuBackground->update(dtSeconds);
    _guiContainer.update(dtSeconds);
    _trackGenerator.update(dtSeconds);

    if(_input.checkActive())
    {
        _input.update(dtSeconds);
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
    _players->handleEvent(event);
    return true;
}
