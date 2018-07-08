#pragma once

#include <functional>
#include <memory>
#include <vector>

#include <ecs/Entity.hpp>

struct Font;
struct MenuItem;
struct TransformNode;


class MenuElement
{
    public:
        ~MenuElement();
        MenuElement& add_button();
        MenuElement& add_text();

        void set_position(float x, float y);
        void set_scale(float x, float y);
        void set_text(const std::string& text);
        void set_on_click(const std::function<void(MenuElement&)>& callback);
        void set_font(const Font& font);

        void select();

    private:
        friend class Menu;
        MenuElement();

        MenuElement& add_element();
        bool is_root() const;


        ecs::Entity _entity;
        MenuElement* _parent;
        std::vector<std::unique_ptr<MenuElement>> _children;
        MenuItem& _item;
        TransformNode& _transform;
};
