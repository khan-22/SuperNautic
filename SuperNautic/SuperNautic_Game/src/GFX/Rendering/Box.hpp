#pragma once

#ifndef BOX_HPP
#define BOX_HPP

#include "GFX/Resources/Model.hpp"
#include "GFX/Rendering/Renderable3D.hpp"
#include "Core/Geometry/BoundingBox.hpp"


namespace GFX
{


	class Box : public Renderable3D
	{
    public:
        Box();
        Box(const BoundingBox& box);

        void render(RenderStates& states) override;
        void setBox(const BoundingBox& box);

    private:
        BoundingBox _box;
        std::shared_ptr<Model> _model;

        std::shared_ptr<Model> generateModel() const;
	};

}

#endif //BOX_HPP
