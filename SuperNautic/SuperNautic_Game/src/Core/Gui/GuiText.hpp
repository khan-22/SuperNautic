#pragma once

#ifndef GUI_TEXT_HPP
#define GUI_TEXT_HPP


#include <functional>

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Text.hpp"

#include "Core/Gui/GuiElement.hpp"
#include "Core/Asset/LoadAssetFunctions.hpp"

class GuiText : public GuiElement
{
    public:
        GuiText(const std::string& str, FontAsset font);

        void setText(const std::string& str);
        const std::string getText() const;

        virtual sf::FloatRect getBoundingRect() const override;

        void setFillColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
        void setOutlineThickness(float thickness);
        void setOutlinecolor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
        void setCharacterSize(unsigned int size);

    private:
        FontAsset _font;
        sf::Text _text;


        virtual void select() override;
        virtual void deselect() override;

        void renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif //GUI_TEXT_HPP
