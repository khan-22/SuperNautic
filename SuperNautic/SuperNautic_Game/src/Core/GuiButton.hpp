#pragma once

#ifndef GUI_BUTTON_HPP
#define GUI_BUTTON_HPP


#include "SFML/Graphics/Text.hpp"

#include "GuiElement.hpp"

class GuiButton : public GuiElement
{
    public:
        GuiButton(const sf::Text& text, std::function<void()> callback);

        bool bIsActivatable() const override;
        bool bIsSelectable() const override;
        sf::FloatRect getBoundingRect() const override;

    private:
        sf::Text _text;
        std::function<void()> _callback;

        void renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
        void select() override;
        void deselect() override;
        void activate() override;

};

#endif //GUI_BUTTON_HPP
