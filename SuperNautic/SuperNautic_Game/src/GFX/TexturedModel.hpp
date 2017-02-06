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
        TexturedModel(const ModelAsset& model, const MaterialAsset& material);

        virtual void render(RenderStates& states) override;

        void setModel(const ModelAsset& model);
        void setMaterial(const MaterialAsset& material);
        void setModelAndMaterial(const ModelAsset& model, const MaterialAsset& material);

    private:
        ModelAsset _model;
        MaterialAsset _material;
        void (TexturedModel::*_renderFunc)(RenderStates&) = nullptr;


        void renderStandard(RenderStates& states);
        void renderModelless(RenderStates& states);
        void renderTextureless(RenderStates& states);
	};

}

#endif //TEXTURED_MODEL_HPP