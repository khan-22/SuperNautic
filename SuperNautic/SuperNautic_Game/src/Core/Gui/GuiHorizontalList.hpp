#pragma once

#ifndef GUI_HORIZONTAL_LIST_HPP
#define GUI_HORIZONTAL_LIST_HPP


#include <functional>

#include "Core/Gui/GuiContainer.hpp"

class GuiHorizontalList : public GuiContainer
{
    public:
        GuiHorizontalList(size_t maxVisibleElements = 5);

        bool bIsSelectable() const override;

        virtual void insert(std::unique_ptr<GuiElement>& element) override;
        virtual void insert(std::vector<std::unique_ptr<GuiElement>>& elements) override;

    private:
        size_t _maxVisibleElements;
        std::vector<GuiElement*> _drawElements;
        float _maxElementWidth = 0.f;
        float _marginBetweenElements = 0.f;
        float _elementStepX = 0.f;
        float _maxElementHeight = 0.f;

        virtual void handleEventCurrent(const sf::Event& event) override;
        virtual void onElementSelect() override;
        virtual void renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual void updateSize() override;
        virtual void select() override;
        virtual void deselect() override;
//        virtual void activate() override;
//        virtual void deactivate() override;
};

#endif //GUI_HORIZONTAL_LIST_HPP
