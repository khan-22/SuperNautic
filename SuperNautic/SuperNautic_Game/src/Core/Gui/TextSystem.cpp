#include "TextSystem.hpp"

#include <ecs/ecs.hpp>

#include "Text.hpp"
#include "WorldTransform.hpp"
#include <GFX/Rendering/SfmlRenderer.hpp>

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

            text->text.setScale(p2 - p1);
            sf::FloatRect bounds = text->text.getGlobalBounds();
            text->text.setPosition(p1.x - bounds.width / 2.f, p1.y - bounds.height / 2.f);
        }
    }

    for(Text* text : ecs::get_components_with<Text>())
    {
        renderer.render(text->text);
    }
}
