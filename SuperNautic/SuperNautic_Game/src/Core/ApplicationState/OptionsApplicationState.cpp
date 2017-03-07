#include <iostream>

#include <windows.h>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/Event.hpp"

#include "Core/ApplicationState/OptionsApplicationState.hpp"
#include "Core/ApplicationState/MainMenuApplicationState.hpp"
#include "Core/ApplicationState/ApplicationStateStack.hpp"
#include "Core/ApplicationState/ApplicationContext.hpp"
#include "Core/Asset/AssetCache.hpp"
#include "Core/Gui/GuiButton.hpp"
#include "Core/Gui/GuiHorizontalList.hpp"
#include "Core/Gui/GuiSlider.hpp"

#include "Core/Asset/LoadAssetFunctions.hpp"

OptionsApplicationState::OptionsApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
: ApplicationState(stack, context)
, _font(AssetCache<sf::Font, std::string>::get("res/arial.ttf"))
, _videoOptions(context.window)
, _toolTip(_font)
, _title("Options", _font)
{
    std::vector<std::unique_ptr<GuiElement>> guiElements;

    GuiHorizontalList* resolutionList = new GuiHorizontalList(3);
    for(const glm::ivec2& resolution : _videoOptions.getAllowedResolutions())
    {
        std::string text = std::to_string(resolution.x) + "x" + std::to_string(resolution.y);
        GuiButton* buttonPtr = new GuiButton(sf::Text(text, *_font.get()), [&]()
        {
            glm::ivec2 localResolution = resolution;
            _videoOptions.setResolution(localResolution);
            applyOptions();
        });
        std::unique_ptr<GuiElement> button(buttonPtr);
        resolutionList->insert(button);

        if(resolution == _videoOptions.getResolution())
        {
            resolutionList->GuiContainer::select((GuiElement*)buttonPtr);
        }
    }

    GuiText* resLabel = new GuiText("Resolution", FontCache::get("res/arial.ttf"));
    resLabel->setOrigin(resLabel->getBoundingRect().width / 2.f, resLabel->getBoundingRect().height);
    sf::FloatRect resBounds = resolutionList->getBoundingRect();
    resLabel->setPosition(resBounds.left + resBounds.width / 2.f, resBounds.top - resLabel->getBoundingRect().height / 2.f);

    resolutionList->registerOnSelect([resLabel](){resLabel->toggleSelection();});
    resolutionList->registerOnDeselect([resLabel](){resLabel->toggleSelection();});

    std::unique_ptr<SceneNode> resLabelPtr(resLabel);
    resolutionList->attachChild(resLabelPtr);



    guiElements.emplace_back(resolutionList);

    GuiButton* fs = new GuiButton(sf::Text("Toggle Fullscreen", *_font.get()), [&]()
    {
        _videoOptions.setFullscreen(!_videoOptions.bIsFullscreen());
        applyOptions();
    });
    guiElements.emplace_back(fs);


    GuiSlider* musicVolume = new GuiSlider
    (
        0.f, 1.f, 50.f, 11,
        sf::Text("Music volume    Low", *_font.get()),
        sf::Text("High", *_font.get())
    );
    musicVolume->setValue(_audioOptions.getMusicVolume());
    musicVolume->setOnChange([this](float v)
    {
        _audioOptions.setMusicVolume(v);
        _context.audio.setVolume(v);
    });
    guiElements.emplace_back(musicVolume);

    GuiSlider* effectsVolume = new GuiSlider
    (
        0.f, 1.f, 50.f, 11,
        sf::Text("Effects volume    Low", *_font.get()),
        sf::Text("High", *_font.get())
    );
    effectsVolume->setValue(_audioOptions.getEffectsVolume());
    effectsVolume->setOnChange([this](float v)
    {
        _audioOptions.setEffectsVolume(v);
        GuiElement::setStepVolume(v);
    });
    guiElements.emplace_back(effectsVolume);

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


    _toolTip.centerAt(static_cast<size_t>(windowSize.x / 2.f), static_cast<size_t>(windowSize.y * 0.95f));
    _toolTip.registerTip(resolutionList, "Set video resolution. Press A to apply.");
    _toolTip.registerTip(fs, "Enable/disable fullscreen.");
    _toolTip.registerTip(backButton, "Go back to main menu.");
    _toolTip.setCharacterSize(18);


    _title.setCharacterSize(50);
    _title.setOrigin(_title.getBoundingRect().width / 2.f, _title.getBoundingRect().height / 2.f);
    _title.setPosition(windowSize.x / 2.f, _guiContainer.getBoundingRect().top / 2.f);

    _guiContainer.toggleSelection();
}

void OptionsApplicationState::render()
{
    _sfmlRenderer.render(*_context.menuBackground);
    _sfmlRenderer.render(_guiContainer);
    _sfmlRenderer.render(_toolTip);
    _sfmlRenderer.render(_title);
    _sfmlRenderer.display(_context.window);
}

bool OptionsApplicationState::bUpdate(float dtSeconds)
{
    _context.menuBackground->update(dtSeconds);

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
                case sf::Keyboard::Escape:
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

bool OptionsApplicationState::bHandleEvent(const sf::Event& event)
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

void OptionsApplicationState::applyOptions()
{
    _videoOptions.recreateWindow();

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
	_context.segmentHandler.reset();
	_context.obstacleHandler.reset();

    _context.segmentHandler.reset(new SegmentHandler("Segments/segmentinfos1.txt", "Segments/ConnectionTypes.txt"));
	_context.obstacleHandler.reset(new ObstacleHandler("obstacleinfo.txt"));

    sf::Vector2u size = _context.window.getSize();

    _context.menuBackground.reset(new MenuBackground(size.x, size.y));
    _guiContainer.setPosition(size.x / 2.f, size.y / 2.f);
    _toolTip.centerAt(static_cast<size_t>(size.x / 2.f), static_cast<size_t>(size.y * 0.95f));
}


