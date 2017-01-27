#include <iostream>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"

#include "PlayApplicationState.hpp"
#include "ApplicationStateStack.hpp"
#include "ApplicationContext.hpp"
#include "AssetCache.hpp"
#include "GuiButton.hpp"
#include "../GFX/SfmlRenderer.hpp"
#include "Camera.h"


PlayApplicationState::PlayApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
: ApplicationState(stack, context)
{
    _guiContainer.setBackground(sf::Color::Magenta);



    static Asset<sf::Font> arialFont = AssetCache<sf::Font, std::string>::get("res/arial.ttf");
    sf::Text text("Click me!", *arialFont.get());
    text.setFillColor(sf::Color::White);
    auto button1 = std::unique_ptr<GuiElement>(new GuiButton(text, [](){std::cout << "Clicky clicky 1!" << std::endl;}));
    auto button2 = std::unique_ptr<GuiElement>(new GuiButton(text, [](){std::cout << "Clicky clicky 2!" << std::endl;}));
    button2->move(0.f, button1->getBoundingRect().height * 1.5f);
    _guiContainer.insert(button1);
    _guiContainer.insert(button2);

    sf::Vector2u windowSize = _context.window.getSize();
    _guiContainer.setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
}

bool PlayApplicationState::bRender()
{
    GFX::SfmlRenderer renderer;
    renderer.render(_guiContainer);
    renderer.display(_context.window, Camera(90, 5, 5));
//    _context.window.draw(_guiContainer);
    return true;
}

bool PlayApplicationState::bUpdate(float dtSeconds)
{
    return true;
}

bool PlayApplicationState::bHandleEvent(const sf::Event& event)
{
    _guiContainer.handleEvent(event);
    return true;
}
