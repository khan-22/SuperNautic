#include "GFX/Resources/Model.hpp"

using namespace GFX;

Model::Model()
	: _tempModelMat(1.f)
{
}


Model::~Model()
{
}

size_t Model::getNumMeshes() const
{
    return _meshes.size();
}

void GFX::Model::setModelMatrix(glm::mat4 ModelMatrix)
{
	_tempModelMat = ModelMatrix;
}


void Model::render(RenderStates& states)
{
    setAttributes(states);
	for (auto& mesh : _meshes)
	{
		mesh->render();
	}
}

void Model::setAttributes(RenderStates& states) const
{
	// TEMP
	static float time = 0.f;
	time += 0.0014f;

	//glm::mat4 model			= glm::rotate(time, glm::vec3(0.f, 1.f, 0.f));

	glm::mat4 view			= states.camera->getView();
	glm::mat4 perspective	= states.camera->getPerspective();

	Shader* shader = states.shader;
	//shader->setUniform("uView",	view);
	shader->setUniform("uModel", _tempModelMat);
	//shader->setUniform("uProjection", perspective);

	glm::mat4 MVP			= perspective * view * _tempModelMat;
	shader->setUniform("uMVP", MVP);

	// TEMP
	//glm::vec4 color(1.f, 0.f, 0.f, 1.f);
	//shader->setUniform("uColor", color);

}

Model::Mesh& Model::addMesh()
{
	_meshes.emplace_back(new Mesh());
	return *_meshes.back();
}


///////////////////////////////////////////////////////////////
////	Model::Mesh
///////////////////////////////////////////////////////////////

Model::Mesh::Mesh()
{
}

Model::Mesh::~Mesh()
{
}

void Model::Mesh::render()
{
	_vao.render();
}

VertexArrayObject& Model::Mesh::getVertexArrayObject()
{
	return _vao;
}

