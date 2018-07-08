#include <iostream>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/Event.hpp"

#include "Core/ApplicationState/MainMenuApplicationState.hpp"
#include "Core/ApplicationState/OptionsApplicationState.hpp"
#include "Core/ApplicationState/ControlsApplicationState.hpp"
#include "Core/ApplicationState/AboutApplicationState.hpp"
#include "Core/ApplicationState/ApplicationStateStack.hpp"
#include "Core/ApplicationState/ApplicationContext.hpp"
#include "Core/ApplicationState/PreGameApplicationState.hpp"
#include "Core/Asset/AssetCache.hpp"
#include "Core/Gui/GuiButton.hpp"
#include "Core/Utility/Camera.h"
#include "GFX/Rendering/SfmlRenderer.hpp"

#include <ecs/ecs.hpp>

struct IsClickable
{
};

struct IsActiveMenuItem
{
};

struct MenuItem
{
    ecs::Entity parent;
    ecs::Entity previous;
    ecs::Entity next;
    ecs::Entity child;
};

struct Origin
{

};

struct Font
{
    FontAsset font = FontCache::get("res/arial.ttf");
    unsigned int char_size = 30;
    sf::Color fill_color = sf::Color::White;
    sf::Color outline_color = sf::Color::White;
    float outline_thickness = 0.f;
};

struct Text
{
    Font font;
    sf::Text text;
};

struct Button
{

};

struct IsSelectable
{

};

struct Transform : public sf::Transform
{
};

struct WorldTransform : public sf::Transform
{
};

struct TransformNode
{
    TransformNode* parent = nullptr;
    Transform* local = nullptr;
    WorldTransform* world = nullptr;
    std::vector<TransformNode*> children;
};

struct Callback
{
    std::function<void()> callback;
};

#include <functional>
class MenuElement
{
    public:
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


class Menu
{
    public:
        Menu();

        MenuElement& get_root();

    private:
        std::unique_ptr<MenuElement> _root;
};

Menu::Menu()
: _root(new MenuElement())
{
}

MenuElement& Menu::get_root()
{
    return *_root;
}


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

namespace
{
    Menu* menu;
}


class MenuItemSystem
{
    public:
        static void handle_event(const sf::Event& event);
        static void update();
};

void MenuItemSystem::handle_event(const sf::Event& event)
{
    if(event.type == sf::Event::EventType::KeyPressed)
    {
        switch(event.key.code)
        {
            case sf::Keyboard::Down:
            {
                ecs::Entity current = ecs::get_entity_with<IsActiveMenuItem, MenuItem>();
                if(!current)
                {
                    break;
                }

                ecs::Entity next = current.get<MenuItem>()->next;
                while(next && !next.has<IsSelectable>())
                {
                    MenuItem* item = next;
                    if(!item)
                    {
                        next = nullptr;
                        break;
                    }
                    next = item->next;
                }
                if(!next)
                {
                    break;
                }

                current.detach<IsActiveMenuItem>();
                next.attach<IsActiveMenuItem>();

                Transform* current_transform = current;
                Transform* next_transform = next;

                if(current_transform)
                {
                    sf::Vector2f p1 = current_transform->transformPoint(0.f, 0.f);
                    sf::Vector2f p2 = current_transform->transformPoint(1.f, 1.f);
                    current_transform->scale(1.f / (p2.x - p1.x), 1.f / (p2.y - p1.y));
                }

                if(next_transform)
                {
                    next_transform->scale(1.2f, 1.2f);
                }
                
                break;
            }
            
            case sf::Keyboard::Up:
            {   
                ecs::Entity current = ecs::get_entity_with<IsActiveMenuItem, MenuItem>();
                if(!current)
                {
                    break;
                }
                ecs::Entity previous = current.get<MenuItem>()->previous;
                while(previous && !previous.has<IsSelectable>())
                {
                    MenuItem* item = previous;
                    if(!item)
                    {
                        previous = nullptr;
                        break;
                    }
                    previous = item->previous;
                }
                if(!previous)
                {
                    break;
                }

                current.detach<IsActiveMenuItem>();
                previous.attach<IsActiveMenuItem>();
                
                Transform* current_transform = current;
                Transform* previous_transform = previous;

                if(current_transform)
                {
                    sf::Vector2f p1 = current_transform->transformPoint(0.f, 0.f);
                    sf::Vector2f p2 = current_transform->transformPoint(1.f, 1.f);
                    current_transform->scale(1.f / (p2.x - p1.x), 1.f / (p2.y - p1.y));
                }

                if(previous_transform)
                {
                    previous_transform->scale(1.2f, 1.2f);
                }

                break;
            }
            case sf::Keyboard::Space:
            {
                Callback* callback = ecs::get_component_with<Callback, IsActiveMenuItem>();
                if(callback)
                {
                    callback->callback();
                }
                break;
            }

            default:
                break;


        }
    }
}

void MenuItemSystem::update()
{

}

class WorldTransformSystem
{
    public:
        static void update();

    private:
        static void update_node(TransformNode& node);
};


void WorldTransformSystem::update_node(TransformNode& node)
{
    assert(node.world);
    for(TransformNode* child : node.children)
    {
        assert(child->world);
        child->world->sf::Transform::operator=(*node.world * *child->world);
    }

    for(TransformNode* child : node.children)
    {
        update_node(*child);
    }
}

void WorldTransformSystem::update()
{
    for(ecs::Entity e : ecs::get_entities_with<Transform, WorldTransform>())
    {
        WorldTransform* world = e;
        Transform* local = e;
        world->sf::Transform::operator=(*local);
    }


    for(TransformNode* node : ecs::get_components_with<TransformNode>())
    {
        if(!node->parent)
        {
            update_node(*node);
        }
    }
}


class TextSystem
{
    public:
        static void handle_event(const sf::Event& event);
        static void update(GFX::SfmlRenderer& renderer);
};

void TextSystem::handle_event(const sf::Event& event)
{
 
}

void TextSystem::update(GFX::SfmlRenderer& renderer)
{
    for(ecs::Entity e : ecs::get_entities_with<Text>())
    {
        Text* text = e;
        text->text.setFont(*text->font.font.get());
        text->text.setCharacterSize(text->font.char_size);
        text->text.setFillColor(text->font.fill_color);
        text->text.setOutlineColor(text->font.outline_color);
        text->text.setOutlineThickness(text->font.outline_thickness);
        
        WorldTransform* transform = e;
        if(transform)
        {
            sf::Vector2f p1 = transform->transformPoint(0.f, 0.f);
            sf::Vector2f p2 = transform->transformPoint(1.f, 1.f);
            text->text.setPosition(p1);
            text->text.setScale(p2 - p1);
        }
    }

    for(Text* text : ecs::get_components_with<Text>())
    {
        renderer.render(text->text);
    }
}



MainMenuApplicationState::MainMenuApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
: ApplicationState(stack, context)
, _input()
{
    menu = new Menu();
    std::cout << "Welcome to MainMenu state." << std::endl;

    MenuElement& root = menu->get_root();
    root.set_position(100.f, 100.f);

    sf::Vector2u windowSize = context.window.getSize();

    MenuElement& title = root.add_text();
    title.set_position(windowSize.x / 2.f, windowSize.y / 4.f);
    
    Font font;
    font.font = FontCache::get("res/basictitlefont.ttf");
    font.char_size = 100;
    font.outline_thickness = 0.f;
    font.fill_color = sf::Color::White;
    title.set_font(font);
    title.set_scale(1.5f, 1.f);
    title.set_text("SUPERNAUTIC");


    MenuElement& play = root.add_button();
    play.set_position(100.f, 100.f);
    play.set_text("Play");
    play.set_on_click([&](MenuElement&)
    {
        _stack.pop();
        _stack.push(std::unique_ptr<ApplicationState>(new PreGameApplicationState(_stack, _context)));
    });
    play.select();

    MenuElement& options = root.add_button();
    options.set_position(100.f, 200.f);
    options.set_text("Options");
    options.set_on_click([&](MenuElement&)
    {
        _stack.clear();
        _stack.push(std::unique_ptr<ApplicationState>(new OptionsApplicationState(_stack, _context)));
    });

    MenuElement& controls = root.add_button();
    controls.set_position(100.f, 300.f);
    controls.set_text("Controls");
    controls.set_on_click([&](MenuElement&)
    {
        _stack.clear();
        _stack.push(std::unique_ptr<ApplicationState>(new ControlsApplicationState(_stack, _context)));
    });

    MenuElement& about = root.add_button();
    about.set_position(100.f, 400.f);
    about.set_text("About");
    about.set_on_click([&](MenuElement&)
    {
        _stack.clear();
        _stack.push(std::unique_ptr<ApplicationState>(new AboutApplicationState(_stack, _context)));
    });

    MenuElement& quit = root.add_button();
    quit.set_position(100.f, 500.f);
    quit.set_text("Quit");
    quit.set_on_click([&](MenuElement&)
    {
        _stack.clear();
    });


}

void MainMenuApplicationState::render()
{
    GFX::SfmlRenderer renderer;
    renderer.render(*_context.menuBackground);
    renderer.render(_guiContainer);

    TextSystem::update(renderer);

    renderer.display(_context.window);
//    _context.window.draw(_guiContainer);
}

bool MainMenuApplicationState::bUpdate(float dtSeconds)
{
    WorldTransformSystem::update();

    _context.menuBackground->update(dtSeconds);

	if (_input.checkActive())
	{
		_input.update(dtSeconds);
		for (const sf::Event& e : _input.getEvents())
		{
			_guiContainer.handleEvent(e);
		}
	}

	int playersFound = 0;
	for (int i = 0; i < 5; i++)
	{
		if (sf::Joystick::isConnected(i)) {
			playersFound++;
		}
	}
	_playersActive = playersFound;
	if (_playersActive == 0)
	{
		_playersActive = 1;
	}
    return true;
}

bool MainMenuApplicationState::bHandleEvent(const sf::Event& event)
{
    _guiContainer.handleEvent(event);
    
    if(event.type == sf::Event::EventType::MouseButtonPressed &&
       event.mouseButton.button == sf::Mouse::Button::Left)
    {
        float x = event.mouseButton.x;
        float y = event.mouseButton.y;
        for(ecs::Entity button : ecs::get_entities_with<std::function<void()>, IsClickable, sf::FloatRect>())
        {
            if(button.get<sf::FloatRect>()->contains(x, y))
            {
                (*button.get<std::function<void()>>())();
                break;
            }
        }
    }

    MenuItemSystem::handle_event(event);
    
    return true;
}
