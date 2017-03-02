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
        const std::string& getText() const;

        virtual sf::FloatRect getBoundingRect() const override;


    private:
        FontAsset _font;
        sf::Text _text;

        void renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif //GUI_TEXT_HPP
