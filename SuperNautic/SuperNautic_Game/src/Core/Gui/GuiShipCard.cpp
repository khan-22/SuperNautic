#include "Core/Gui/GuiShipCard.hpp"
#include "Core/Gui/GuiPlayerJoinContainer.hpp"

GuiShipCard::GuiShipCard(unsigned int id, const GuiPlayerJoinContainer& players)
: _id(id)
, _players(players)
{

}

sf::FloatRect GuiShipCard::getBoundingRect() const
{
	return getWorldTransform().transformRect(_card.getGlobalBounds());
}

void GuiShipCard::renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(_bHasJoined)
    {
        target.draw(_card, states);
        target.draw(_background, states);
    }
}

void GuiShipCard::updateCurrent(float dtSeconds)
{
    std::vector<GuiPlayerJoinContainer::Player> joinedPlayers = _players.getJoinedPlayers();
    auto foundIt = std::find_if(joinedPlayers.begin(), joinedPlayers.end(), [this](const GuiPlayerJoinContainer::Player& p)
    {
        return p.id == _id;
    });
    if(foundIt == joinedPlayers.end())
    {
        _bHasJoined = false;
        return;
    }

    _bHasJoined = true;

    GuiPlayerJoinContainer::Player p = *foundIt;
    if(p.shipId != _shipId)
    {
        _shipId = p.shipId;

        _texture = SFMLTextureCache::get("ship" + std::to_string(p.shipId) + ".png");
        _card.setTexture(_texture.get(), true);

        sf::Vector2f size = sf::Vector2f(_texture.get()->getSize());
        float clipX = std::max(size.x - _maxWidth, 0.f);
        float clipY = std::max(size.y - _maxHeight, 0.f);

        if(clipX > 0.f || clipY > 0.f)
        {
            float widthPerHeight = size.x / size.y;
            size.x = std::min(size.x - clipX, size.x - clipY * widthPerHeight);
            size.y = std::min(size.y - clipY, size.y - clipX / widthPerHeight);
            _card.setSize(size);
        }

        setOrigin(size.x / 2.f, size.y / 2.f);
        setPosition(getPosition());

        _background.setSize(size);
        p.color *= 255.f;
        _background.setFillColor(sf::Color(p.color.r, p.color.g, p.color.b, 100));
    }
}


void GuiShipCard::setMaxSize(float width, float height)
{
    _maxWidth = width;
    _maxHeight = height;
}
