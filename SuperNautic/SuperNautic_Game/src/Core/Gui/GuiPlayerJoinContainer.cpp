#include <cassert>

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Window/Event.hpp"

#include "Core/Gui/GuiPlayerJoinContainer.hpp"
#include "Core/Gui/GuiPlayerJoin.hpp"


const unsigned char GuiPlayerJoinContainer::_COLORS[4][_MAX_PLAYERS] =
{
    {255, 0, 0, 255},
    {0, 255, 0, 255},
    {0, 0, 255, 255},
    {255, 255, 0, 255}
};

GuiPlayerJoinContainer::GuiPlayerJoinContainer()
{
    createWindows();
    setOrigin(getBoundingRect().width / 2.f, getBoundingRect().height / 2.f);
}

void GuiPlayerJoinContainer::createWindows()
{

    sf::Vector2f pos(0.f, 0.f);
    std::vector<std::unique_ptr<GuiElement>> guiElements;
    for(size_t i = 0; i < _MAX_PLAYERS; i++)
    {
        GuiPlayerJoin* p = new GuiPlayerJoin();
        p->setPosition(pos);
        pos.x += p->getBoundingRect().width * 1.5f;

        const unsigned char* color = _COLORS[i];
        p->setColor(color[0], color[1], color[2], color[3]);

        _windows.push_back(p);
        guiElements.emplace_back(p);
    }

    insert(guiElements);
}
#include "Core/Io/Log.hpp"

void GuiPlayerJoinContainer::handleEventCurrent(const sf::Event& event)
{
    switch(event.type)
    {
        case sf::Event::JoystickButtonPressed:
            LOG(event.joystickButton.joystickId, " pressed ", event.joystickButton.button);
            if(event.joystickButton.button == 3)
            {
                toggle(event.joystickButton.joystickId);
            }
            break;

        case sf::Event::JoystickDisconnected:
            LOG(event.joystickButton.joystickId, "disconnected");
            drop(event.joystickButton.joystickId);
            break;

        case sf::Event::JoystickConnected:
            LOG(event.joystickButton.joystickId, "connected");
            break;

        default:
            break;
    }
}


void GuiPlayerJoinContainer::toggle(unsigned int id)
{
    for(auto& j : _windows)
    {
        if(j->bIsAssigned() && j->getId() == id)
        {
            j->toggleJoin();
            return;
        }
    }

    for(auto& j : _windows)
    {
        if(!j->bIsAssigned())
        {
            j->assign(id);
            j->toggleJoin();
            return;
        }
    }
}

void GuiPlayerJoinContainer::drop(unsigned int id)
{
    for(auto& j : _windows)
    {
        if(j->bIsAssigned() && j->getId() == id)
        {
            j->unassign();
            return;
        }
    }
}


std::vector<GuiPlayerJoinContainer::Player> GuiPlayerJoinContainer::getJoinedPlayers() const
{
    std::vector<Player> players;
    assert(_windows.size() == _MAX_PLAYERS);
    for(size_t i = 0; i < _windows.size(); i++)
    {
        const auto& j = _windows[i];
        if(j->bHasJoined())
        {
            Player p;
            for(size_t k = 0; k < 4; k++)
            {
                p.color[static_cast<glm::length_t>(k)] = _COLORS[i][k];
            }
            p.id = j->getId();
            players.push_back(p);
        }
    }

    if(players.empty())
    {
        Player p;
        for(size_t k = 0; k < 4; k++)
        {
            p.color[static_cast<glm::length_t>(k)] = _COLORS[0][k];
        }
        p.id = 0;
        players.push_back(p);
    }

    for(Player& p : players)
    {
        p.color /= 255.f;
    }

    return players;
}
