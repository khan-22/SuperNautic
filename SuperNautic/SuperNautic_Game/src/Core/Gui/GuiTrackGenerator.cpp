#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

#include "Core/Gui/GuiTrackGenerator.hpp"





void setSize(float width, float height)
{

}


std::unique_ptr<Track> GuiTrackGenerator::takeTrack()
{
    return std::move(_track);
}
