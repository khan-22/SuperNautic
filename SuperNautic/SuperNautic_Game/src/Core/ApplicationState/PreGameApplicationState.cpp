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
        _context.players = _players->getJoinedPlayers();
        if(_context.players.empty())
        {
            return;
        }
        _stack.clear();
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
    _title.setPosition(windowSize.x / 2.f, _guiContainer.getBoundingRect().top / 4.f);

    _guiContainer.select(startButton);

    _forwardRenderer.initialize(&_context.window, 0.f, 0.f, 1.f, 1.f, &GFX::Framebuffer::DEFAULT);

    _shipIds = {-1U, -1U, -1U, -1U};
}

void PreGameApplicationState::render()
{
    _sfmlRenderer.render(*_context.menuBackground);

    for(size_t i = 0; i < _shipIds.size(); i++)
    {
        if(_shipIds[i] != -1U)
        {
            assert(i < _shipImages.size());
            _sfmlRenderer.render(_shipImages[i]);
        }
    }

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

    std::vector<GuiPlayerJoinContainer::Player> joinedPlayers = _players->getJoinedPlayers();
    for(const GuiPlayerJoinContainer::Player& p : joinedPlayers)
    {
        assert(p.id < _shipIds.size());
        if(_shipIds[p.id] != p.shipId)
        {
            assert(p.id < _shipImages.size());
            GuiTexture& img = _shipImages[p.id];
            img.setTexture("ship2selection.png");

            sf::Vector2u windowSize = _context.window.getSize();
            sf::FloatRect bounds = img.getBoundingRect();
            float clipX = std::max(bounds.width - windowSize.x / 2.f, 0.f);
            float clipY = std::max(bounds.height - windowSize.y / 2.f, 0.f);



            if(clipX > 0.f || clipY > 0.f)
            {
                float widthPerHeight = bounds.width / bounds.height;
                float newWidth = std::min(bounds.width - clipX, bounds.width - clipY * widthPerHeight);
                float newHeight = std::min(bounds.height - clipY, bounds.height - clipX / widthPerHeight);
                img.setSize(newWidth, newHeight);
                bounds = img.getBoundingRect();
            }

            img.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

            float x = (p.id % 2) * (windowSize.x / 2.f) + windowSize.x / 4.f;
            float y = (p.id / 2) * (windowSize.y / 2.f) + windowSize.y / 4.f;
            img.setPosition(sf::Vector2f(x, y));

            _shipIds[p.id] = p.shipId;
        }
    }

    for(size_t i = 0; i < _shipIds.size(); i++)
    {
        auto foundIt = std::find_if(joinedPlayers.begin(), joinedPlayers.end(), [i](const GuiPlayerJoinContainer::Player& p)
        {
            return p.id == i;
        });
        if(foundIt == joinedPlayers.end())
        {
            _shipIds[i] = -1U;
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

        case sf::Keyboard::Num1:
        {
            sf::Event e;
            e.type = sf::Event::JoystickButtonPressed;
            e.joystickButton.joystickId = 0;
            e.joystickButton.button = 3;
            _players->handleEvent(e);
            break;
        }
        case sf::Keyboard::Num2:
        {
            sf::Event e;
            e.type = sf::Event::JoystickButtonPressed;
            e.joystickButton.joystickId = 1;
            e.joystickButton.button = 3;
            _players->handleEvent(e);
            break;
        }
        case sf::Keyboard::Num3:
        {
            sf::Event e;
            e.type = sf::Event::JoystickButtonPressed;
            e.joystickButton.joystickId = 2;
            e.joystickButton.button = 3;
            _players->handleEvent(e);
            break;
        }
        case sf::Keyboard::Num4:
        {
            sf::Event e;
            e.type = sf::Event::JoystickButtonPressed;
            e.joystickButton.joystickId = 3;
            e.joystickButton.button = 3;
            _players->handleEvent(e);
            break;
        }

        case sf::Keyboard::Num5:
        {
            sf::Event e;
            e.type = sf::Event::JoystickButtonPressed;
            e.joystickButton.joystickId = 0;
            e.joystickButton.button = 2;
            _players->handleEvent(e);
            break;
        }
        case sf::Keyboard::Num6:
        {
            sf::Event e;
            e.type = sf::Event::JoystickButtonPressed;
            e.joystickButton.joystickId = 1;
            e.joystickButton.button = 2;
            _players->handleEvent(e);
            break;
        }
        case sf::Keyboard::Num7:
        {
            sf::Event e;
            e.type = sf::Event::JoystickButtonPressed;
            e.joystickButton.joystickId = 2;
            e.joystickButton.button = 2;
            _players->handleEvent(e);
            break;
        }
        case sf::Keyboard::Num8:
        {
            sf::Event e;
            e.type = sf::Event::JoystickButtonPressed;
            e.joystickButton.joystickId = 3;
            e.joystickButton.button = 2;
            _players->handleEvent(e);
            break;
        }

        default:
            break;
        }
    }
    _guiContainer.handleEvent(event);
    _players->handleEvent(event);
    return true;
}
