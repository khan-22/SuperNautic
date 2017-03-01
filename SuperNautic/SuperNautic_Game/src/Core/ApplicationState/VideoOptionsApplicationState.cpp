#include <iostream>

#include <windows.h>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/Event.hpp"

#include "Core/ApplicationState/VideoOptionsApplicationState.hpp"
#include "Core/ApplicationState/MainMenuApplicationState.hpp"
#include "Core/ApplicationState/ApplicationStateStack.hpp"
#include "Core/ApplicationState/ApplicationContext.hpp"
#include "Core/Asset/AssetCache.hpp"
#include "Core/Gui/GuiButton.hpp"
#include "Core/Utility/WindowConstants.hpp"
#include "Core/Asset/LoadAssetFunctions.hpp"
#include "Core/Utility/FileUtility.hpp"

size_t VideoOptionsApplicationState::_DEFAULT_RESOLUTION_X = 1280;
size_t VideoOptionsApplicationState::_DEFAULT_RESOLUTION_Y = 720;

VideoOptionsApplicationState::VideoOptionsApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
: ApplicationState(stack, context)
, _font(AssetCache<sf::Font, std::string>::get("res/arial.ttf"))
{
    readConfig();

    std::vector<std::unique_ptr<GuiElement>> guiElements;

    GuiButton* set1920x1080 = new GuiButton(sf::Text("1920x1080", *_font.get()), [&]()
    {
        setResolution(1920, 1080);
        writeConfig();
        recreateWindow();
    });
    guiElements.emplace_back(set1920x1080);

    GuiButton* set1280x720 = new GuiButton(sf::Text("1280x720", *_font.get()), [&]()
    {
        setResolution(1280, 720);
        writeConfig();
        recreateWindow();
    });
    guiElements.emplace_back(set1280x720);

    GuiButton* fs = new GuiButton(sf::Text("toggle fs", *_font.get()), [&]()
    {
        setFullscreen(!(_windowStyle & sf::Style::Fullscreen));
        writeConfig();
        recreateWindow();
    });
    guiElements.emplace_back(fs);


    GuiButton* backButton = new GuiButton(sf::Text("Back", *_font.get()), [&]()
    {
        _stack.clear();
        _stack.push(std::unique_ptr<ApplicationState>(new MainMenuApplicationState(_stack, _context)));
    });
    guiElements.emplace_back(backButton);


    sf::Vector2f pos(0.f, -guiElements.front()->getBoundingRect().height);
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
    _guiContainer.toggleSelection();
}

void VideoOptionsApplicationState::render()
{
    _sfmlRenderer.render(_guiContainer);
    _sfmlRenderer.display(_context.window);
}

bool VideoOptionsApplicationState::bUpdate(float dtSeconds)
{
    _guiContainer.update();

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
                    if(!_guiContainer.bIsActive())
                    {
                        _stack.clear();
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

bool VideoOptionsApplicationState::bHandleEvent(const sf::Event& event)
{
    _guiContainer.handleEvent(event);
    return true;
}

void VideoOptionsApplicationState::setResolution(size_t x, size_t y)
{
    _resolutionX = x;
    _resolutionY = y;
}

void VideoOptionsApplicationState::setFullscreen(bool bSetFullscreen)
{
    if(bSetFullscreen)
    {
        _windowStyle |= sf::Style::Fullscreen;
    }
    else
    {
        _windowStyle &= ~sf::Style::Fullscreen;
    }
}

void VideoOptionsApplicationState::recreateWindow()
{
    sf::ContextSettings settings = _context.window.getSettings();
    _context.window.create(sf::VideoMode(_resolutionX, _resolutionY), WindowConstants::TITLE, _windowStyle, settings);

    _context.track.reset();

    _context.obstacleHandler.reset();
    _context.segmentHandler.reset();

    ShaderCache::reload();
    ModelCache::reload();
    TextureCache::reload();

    // These caches below don't actually manage OpenGL resources and
    // don't have to be reloaded.
//    RawMeshCache::reload();
//    MaterialCache::reload();

    // SegmentHandler and ObstacleHandler have to be reloaded
    // manually for some reason.
    // TODO: Ask Timmie about it.
    _context.segmentHandler.reset(new SegmentHandler("Segments/segmentinfos4.txt", "Segments/ConnectionTypes.txt"));
	_context.obstacleHandler.reset(new ObstacleHandler("obstacleinfo.txt"));

	for(size_t i = 0; i < _context.segmentHandler->infos().size(); i++)
    {
        _context.segmentHandler->loadSegment(i);
    }
}

void VideoOptionsApplicationState::readConfig()
{
    createFileIfNotExists(_CONFIG_PATH);
    _config.read(_CONFIG_PATH);
    size_t resolutionX;
    size_t resolutionY;
    bool bFullscreen;

    if
    (
        !_config.get("resX", _resolutionX) ||
        !_config.get("resY", _resolutionY) ||
        !_config.get("fullscreen", bFullscreen)
    )
    {
        rebuildConfig(_config);
        readConfig();
        return;
    }

    setFullscreen(bFullscreen);
}

void VideoOptionsApplicationState::rebuildConfig(ConfigFile& config)
{
    config.set("resX", _DEFAULT_RESOLUTION_X);
    config.set("resY", _DEFAULT_RESOLUTION_Y);
    config.set("fullscreen", bool(_DEFAULT_WINDOW_STYLE & sf::Style::Fullscreen));
    config.write(_CONFIG_PATH);
}

void VideoOptionsApplicationState::writeConfig()
{
    _config.set("resX", _resolutionX);
    _config.set("resY", _resolutionY);
    _config.set("fullscreen", bool(_windowStyle & sf::Style::Fullscreen));
    _config.write(_CONFIG_PATH);
}
