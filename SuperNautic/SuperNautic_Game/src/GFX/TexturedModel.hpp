#pragma once

#ifndef TEXTURED_MODEL_HPP
#define TEXTURED_MODEL_HPP

#include "Renderable3D.hpp"
#include "../Core/LoadAssetFunctions.hpp"

namespace GFX
{


	class TexturedModel : public Renderable3D
	{
    public:
        TexturedModel();
        TexturedModel(ModelAsset& model, MaterialAsset& material);

        virtual void render(RenderStates& states) override;
        void setModelAndMaterial(ModelAsset& model, MaterialAsset& material);

    private:
        ModelAsset _model;
        MaterialAsset _material;
	};

}

#endif //TEXTURED_MODEL_HPP
