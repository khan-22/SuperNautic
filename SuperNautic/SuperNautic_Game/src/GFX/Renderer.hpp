#pragma once

#ifndef RENDERER_HPP
#define RENDERER_HPP

class Camera;

namespace GFX
{
    template<typename DrawT, typename TransformT>
    class Renderer
    {
    public:

        void render(const DrawT& draw, const TransformT& transform);
        void display();
        void setCamera(const Camera& camera);

    };
}


#endif //RENDERER_HPP
