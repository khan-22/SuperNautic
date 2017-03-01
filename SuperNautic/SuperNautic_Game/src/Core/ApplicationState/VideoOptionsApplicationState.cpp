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


VideoOptionsApplicationState::VideoOptionsApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
: ApplicationState(stack, context)
, _font(AssetCache<sf::Font, std::string>::get("res/arial.ttf"))
, _windowStyle(sf::Style::Titlebar | sf::Style::Close)
, _resolutionX(1280)
, _resolutionY(720)
{

    std::vector<std::unique_ptr<GuiElement>> guiElements;

    GuiButton* set1920x1080 = new GuiButton(sf::Text("1920x1080", *_font.get()), [&]()
    {
        setResolution(1920, 1080);
        recreateWindow();
    });
    guiElements.emplace_back(set1920x1080);

    GuiButton* set1280x720 = new GuiButton(sf::Text("1280x720", *_font.get()), [&]()
    {
        setResolution(1280, 720);
        recreateWindow();
    });
    guiElements.emplace_back(set1280x720);

    GuiButton* fs = new GuiButton(sf::Text("toggle fs", *_font.get()), [&]()
    {
        setFullscreen(!(_windowStyle & sf::Style::Fullscreen));
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
//    HGLRC oldContext = wglGetCurrentContext();
//    assert(oldContext != NULL);
//    HGLRC newContext = wglCreateContext(GetDC(_context.window.getSystemHandle()));
//    assert(newContext != NULL);
//    bool result = wglCopyContext(oldContext, newContext, )

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

    _context.segmentHandler.reset(new SegmentHandler("Segments/segmentinfos4.txt", "Segments/ConnectionTypes.txt"));
	_context.obstacleHandler.reset(new ObstacleHandler("obstacleinfo.txt"));

	for(size_t i = 0; i < _context.segmentHandler->infos().size(); i++)
    {
        _context.segmentHandler->loadSegment(i);
    }


}
