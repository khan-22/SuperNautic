#pragma once

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <list>

namespace sf
{
    class RenderTarget;
}

class Camera;

namespace GFX
{
    template<typename DrawT>
    class Renderer
    {
    public:
        void render(const DrawT& draw);
        void display(sf::RenderTarget& target, const Camera& camera);

    private:
        std::list<const DrawT*> _drawCalls;
    };
}


#endif //RENDERER_HPP
