#pragma once

#ifndef BOX_HPP
#define BOX_HPP

#include "Model.hpp"
#include "Renderable3D.hpp"
#include "../Core/Geometric Primitives/BoundingBox.hpp"


namespace GFX
{


	class Box : public Renderable3D
	{
    public:
        Box();
        Box(const BoundingBox& box);

        void render(RenderStates& states) override;
        void setBox(const BoundingBox& box);
        const BoundingBox& getBox() const;

    private:
        BoundingBox _box;
        std::shared_ptr<Model> _model;

        std::shared_ptr<Model> generateModel() const;
	};

}

#endif //BOX_HPP
