#include "Model.hpp"

#include <glm/gtx/transform.hpp>

using namespace GFX;


Model::Model()
{
}


Model::~Model()
{
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
	time += 0.0007f;

	//glm::mat4 model			= glm::rotate(time, glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 model = glm::mat4();

	glm::mat4 view			= states.camera->getView();
	glm::mat4 perspective	= states.camera->getPerspective();

	Shader* shader = states.shader;
	shader->setUniform("uModel", model);
	shader->setUniform("uView",	view);
	shader->setUniform("uProjection", perspective);

	// TEMP
	glm::vec4 color(1.f, 0.f, 0.f, 1.f);
	shader->setUniform("uColor", color);

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

