#include "GFX/Resources/TexturedModel.hpp"

#include <cassert>

using namespace GFX;

TexturedModel::TexturedModel(const ModelAsset& model, const MaterialAsset& material)
{
    setModelAndMaterial(model, material);
}

TexturedModel::TexturedModel()
{
    setModelAndMaterial(ModelAsset(), MaterialAsset());
}


void TexturedModel::render(RenderStates& states)
{
    (this->*_renderFunc)(states);
}

void TexturedModel::renderStandard(RenderStates& states)
{
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

void TexturedModel::renderModelless(RenderStates& states)
{
    LOG_ERROR("Trying to render without a model.");
}

void TexturedModel::renderTextureless(RenderStates& states)
{
    Model& model = *_model.get();
    model.setAttributes(states);
    for(size_t i = 0; i < model._meshes.size(); i++)
    {
        model._meshes[i]->render();
    }
}

void TexturedModel::setModel(const ModelAsset& model)
{
    setModelAndMaterial(model, _material);
}

void TexturedModel::setMaterial(const MaterialAsset& material)
{
    setModelAndMaterial(_model, material);
}

void TexturedModel::setModelAndMaterial(const ModelAsset& model, const MaterialAsset& material)
{
    _model = model;
    _material = material;

    if(model.get() == nullptr)
    {
        _renderFunc = &TexturedModel::renderModelless;
        return;
    }

    if(material.get() == nullptr)
    {
        _renderFunc = &TexturedModel::renderTextureless;
        return;
    }

    if(material.get()->getNumGroups() < model.get()->getNumMeshes())
    {
        _renderFunc = &TexturedModel::renderTextureless;
        return;
    }

    _renderFunc = &TexturedModel::renderStandard;
}

ModelAsset GFX::TexturedModel::getModelAsset()
{
	return _model;
}
