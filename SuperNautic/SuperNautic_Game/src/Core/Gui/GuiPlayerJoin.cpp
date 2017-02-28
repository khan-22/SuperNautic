#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Window/Event.hpp"

#include "Core/Gui/GuiPlayerJoin.hpp"

GuiPlayerJoin::GuiPlayerJoin(unsigned char playerId)
: _id(playerId)
, _input(playerId)
{
    leave();

    _window.setSize(sf::Vector2f(50.f, 50.f));
    _window.setOutlineThickness(5.f);

    sf::Color defaultColor = sf::Color::Magenta;
    setColor(defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a);
}

void GuiPlayerJoin::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    _color = sf::Color(r, g, b, a);
    _window.setOutlineColor(_color);
    if(_bHasJoined)
    {
        _window.setFillColor(_color);
    }
}

void GuiPlayerJoin::renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_window, states);
}

sf::FloatRect GuiPlayerJoin::getBoundingRect() const
{
    return getWorldTransform().transformRect(_window.getGlobalBounds());
}

void GuiPlayerJoin::updateCurrent()
{
    _input.update();
    for(const sf::Event& e : _input.getEvents())
    {
        if(e.type == sf::Event::KeyPressed)
        {
            switch(e.key.code)
            {
//                case sf::Keyboard::A:
//                    join();
//                    break;

                case sf::Keyboard::Y:
//                    leave();
                    if(_bHasJoined)
                    {
                        leave();
                    }
                    else
                    {
                        join();
                    }
                    break;

                default:
                    break;
            }
        }
    }
}

void GuiPlayerJoin::join()
{
    _window.setFillColor(_color);
    _bHasJoined = true;
    _joinCallback(_id);
}

void GuiPlayerJoin::leave()
{
    _window.setFillColor(sf::Color::Transparent);
    _bHasJoined = false;
    _leaveCallback(_id);
}


void GuiPlayerJoin::setOnJoin(const std::function<void(unsigned char)>& callback)
{
    _joinCallback = callback;
}

void GuiPlayerJoin::setOnLeave(const std::function<void(unsigned char)>& callback)
{
    _leaveCallback = callback;
}
