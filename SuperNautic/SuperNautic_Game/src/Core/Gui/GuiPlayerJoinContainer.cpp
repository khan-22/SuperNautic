#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Window/Event.hpp"

#include "Core/Gui/GuiPlayerJoinContainer.hpp"
#include "Core/Gui/GuiPlayerJoin.hpp"

GuiPlayerJoinContainer::GuiPlayerJoinContainer()
{
    createWindows();
    setOrigin(getBoundingRect().width / 2.f, getBoundingRect().height / 2.f);
}

void GuiPlayerJoinContainer::createWindows()
{
    static constexpr size_t MAX_PLAYERS = 4;

    static constexpr unsigned char COLORS[4][MAX_PLAYERS] =
    {
        {255, 0, 0, 255},
        {0, 255, 0, 255},
        {0, 0, 255, 255},
        {255, 255, 0, 255}
    };

    sf::Vector2f pos(0.f, 0.f);
    std::vector<std::unique_ptr<GuiElement>> joins;
    for(size_t i = 0; i < MAX_PLAYERS; i++)
    {
		int ids[4] = {0, 3, 4, 1};
        GuiPlayerJoin* p = new GuiPlayerJoin(i);

        p->setPosition(pos);
        pos.x += p->getBoundingRect().width * 1.5f;

        const unsigned char* color = COLORS[i];
        p->setColor(color[0], color[1], color[2], color[3]);

        joins.emplace_back(p);
        _windows.push_back(p);
    }

    insert(joins);
}

void GuiPlayerJoinContainer::setOnJoin(const std::function<void(unsigned char)>& callback)
{
    for(GuiPlayerJoin* window : _windows)
    {
        window->setOnJoin(callback);
    }
}

void GuiPlayerJoinContainer::setOnLeave(const std::function<void(unsigned char)>& callback)
{
    for(GuiPlayerJoin* window : _windows)
    {
        window->setOnLeave(callback);
    }
}
