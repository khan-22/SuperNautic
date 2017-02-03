#include <cassert>

#include "TexturedModel.hpp"

using namespace GFX;

TexturedModel::TexturedModel(ModelAsset& model, MaterialAsset& material)
{
    setModelAndMaterial(model, material);
}

TexturedModel::TexturedModel()
{
}


void TexturedModel::render(RenderStates& states)
{
    assert(_model.get() != nullptr);
    assert(_material.get() != nullptr);

    Model& model = *_model.get();
    model.setAttributes(states);
    for(size_t i = 0; i < model._meshes.size(); i++)
    {
        const Material& material = *_material.get();
        material.bind(i);
        model._meshes[i]->render();
        material.unbind(i);
    }
}

void TexturedModel::setModelAndMaterial(ModelAsset& model, MaterialAsset& material)
{
    if(model.get() == nullptr || material.get() == nullptr)
    {
        LOG_ERROR("Received nullptr on model or material.");
        return;
    }

    _model = model;
    _material = material;
}
