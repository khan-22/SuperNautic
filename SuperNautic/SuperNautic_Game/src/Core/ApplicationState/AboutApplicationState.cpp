#include <iostream>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"

#include "Core/ApplicationState/AboutApplicationState.hpp"
#include "Core/ApplicationState/MainMenuApplicationState.hpp"
#include "Core/ApplicationState/ApplicationStateStack.hpp"
#include "Core/ApplicationState/ApplicationContext.hpp"
#include "Core/Asset/AssetCache.hpp"
#include "GFX/Rendering/SfmlRenderer.hpp"


AboutApplicationState::AboutApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
: ApplicationState(stack, context)
, _font(FontCache::get("res/arial.ttf"))
{
    sf::Vector2u windowSize = _context.window.getSize();

    GuiText* title = new GuiText("About", _font);
    title->setCharacterSize(50);
    title->setOrigin(title->getBoundingRect().width / 2.f, title->getBoundingRect().height / 2.f);
    title->setPosition(windowSize.x / 2.f, windowSize.y / 4.f);
    auto titlePtr = std::unique_ptr<SceneNode>(title);
    _guiGraph.attachChild(titlePtr);




    std::stringstream aboutText;
    aboutText   << "SuperNautic is an underwater racing game developed by a team of students" << std::endl
                << " at Blekinge Institute of Technology for an 8 weeks long project." << std::endl;
    auto about = std::unique_ptr<SceneNode>(new GuiText(aboutText.str(), _font));
    about->setOrigin(about->getBoundingRect().width / 2.f, about->getBoundingRect().height / 2.f);
    about->setPosition(title->getPosition());
    about->move(0.f, title->getBoundingRect().height * 2.5f);

    auto theTeam = std::unique_ptr<SceneNode>(new GuiText("THE TEAM", _font));
    theTeam->setOrigin(theTeam->getBoundingRect().width / 2.f, theTeam->getBoundingRect().height / 2.f);
    theTeam->setPosition(about->getPosition());
    theTeam->move(0.f, about->getBoundingRect().height * 0.8f);


    static const std::vector<std::string> AUTHORS =
    {
        "Fritjof Cavallin",
        "Karl Hansson",
        "Mikael Hernvall",
        "Joakim Jerkenhag",
        "Timmie Pettersson",
        "Peyman Torabi"
    };
    sf::Vector2f pos = theTeam->getPosition();
    pos.y += theTeam->getBoundingRect().height * 1.5f;
    for(const std::string& authorStr : AUTHORS)
    {
        auto author = std::unique_ptr<SceneNode>(new GuiText(authorStr, _font));
        author->setOrigin(author->getBoundingRect().width / 2.f, author->getBoundingRect().height / 2.f);
        author->setPosition(pos);
        pos.y += 40.f;

        _guiGraph.attachChild(author);
    }
    _guiGraph.attachChild(about);
    _guiGraph.attachChild(theTeam);



}

void AboutApplicationState::render()
{
    GFX::SfmlRenderer renderer;
    renderer.render(*_context.menuBackground);
    renderer.render(_guiGraph);
    renderer.display(_context.window);
}

bool AboutApplicationState::bUpdate(float dtSeconds)
{
    _context.menuBackground->update(dtSeconds);
    return true;
}

bool AboutApplicationState::bHandleEvent(const sf::Event& event)
{
    if(event.type == sf::Event::KeyPressed)
    {
        switch(event.key.code)
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
    if(_input.checkActive())
    {
        _input.update();
        for(const sf::Event& e : _input.getEvents())
        {
            _guiGraph.handleEvent(e);
        }
    }
    return true;
}
