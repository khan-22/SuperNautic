#include "MenuElement.hpp"

#include <ecs/ecs.hpp>

#include "Font.hpp"
#include "TransformNode.hpp"
#include "Transform.hpp"
#include "WorldTransform.hpp"
#include "MenuItem.hpp"
#include "IsSelectable.hpp"
#include "IsActiveMenuItem.hpp"
#include "Text.hpp"
#include "Callback.hpp"

MenuElement::MenuElement()
: _entity(ecs::create_entity())
, _parent(nullptr)
, _item(_entity += MenuItem())
, _transform(_entity += TransformNode())
{
    _transform.parent = nullptr;
    _transform.local = &(_entity += Transform());
    _transform.world = &(_entity += WorldTransform());
}

MenuElement::~MenuElement()
{
    ecs::destroy_entity(_entity);
}


void MenuElement::select()
{
    if(!_entity.has<IsSelectable>())
    {
        return;
    }

    IsActiveMenuItem* i = _entity;
    if(i)
    {
        return;
    }

    _entity += IsActiveMenuItem();

    Transform* transform = _entity;
    transform->scale(1.2f, 1.2f);
}

MenuElement& MenuElement::add_element()
{
    MenuElement* child = new MenuElement();
    
    child->_parent = this;

    child->_transform.parent = &_transform;
    _transform.children.push_back(&child->_transform);

    MenuItem& child_item = child->_item;
    child_item.parent = _entity;
    if(_children.empty())
    {
        _item.child = child->_entity;
        child_item.next = child->_entity;
        child_item.previous = child->_entity;
    }
    else
    {
        child_item.next = _children.front()->_entity;
        child_item.previous = _children.back()->_entity;

        _children.back()->_item.next = child->_entity;
        _children.front()->_item.previous = child->_entity;
    }

    _children.emplace_back(child);
    return *child;
}

MenuElement& MenuElement::add_button()
{
    MenuElement& button = add_element();
    button._entity += IsSelectable();

    button.set_text("");
    Font& font = button._entity.get<Text>()->font;
    font.outline_thickness = 2.f;
    font.outline_color = sf::Color::Black;
    return button;
}

MenuElement& MenuElement::add_text()
{
    MenuElement& text = add_element();
    text.set_text("");
    return text;
}

bool MenuElement::is_root() const
{
    return _parent == nullptr;
}


void MenuElement::set_text(const std::string& text)
{
    Text* t = _entity;
    if(!t)
    {
        t = &(_entity += Text());
    }

    t->text.setString(text);
}

void MenuElement::set_font(const Font& font)
{
    Text* t = _entity;
    if(!t)
    {
        t = &(_entity += Text());
    }

    t->font = font;
}


void MenuElement::set_position(float x, float y)
{
    Transform* transform = _entity;
    sf::Vector2f p = transform->transformPoint(0.f, 0.f);
    transform->translate(x - p.x, y - p.y);
}

void MenuElement::set_on_click(const std::function<void(MenuElement&)>& callback)
{
    Callback* cb = _entity;
    if (!cb)
    {
        cb = &(_entity += Callback());
    }

    cb->callback = [this, callback](){callback(*this);};
}


void MenuElement::set_scale(float x, float y)
{
    Transform* transform = _entity;
    transform->scale(1.f, 1.f);
}