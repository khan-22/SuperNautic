#include "MenuItemSystem.hpp"

#include <SFML/Window/Event.hpp>

#include <ecs/ecs.hpp>

#include "MenuItem.hpp"
#include "IsSelectable.hpp"
#include "Transform.hpp"
#include "Callback.hpp"
#include "IsActiveMenuItem.hpp"

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
            case sf::Keyboard::Enter:
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



