#include "Model.hpp"

using namespace GFX;


Model::Model()
{
}


Model::~Model()
{
}

void GFX::Model::render()
{
	for (auto& mesh : _meshes)
	{
		mesh.render();
	}
}

Model::Mesh& GFX::Model::addMesh()
{
	_meshes.emplace_back();
	return _meshes.back();
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
	_vao.bind();
	_vao.render();
	_vao.unbind();
}

VertexArrayObject& GFX::Model::Mesh::getVertexArrayObject()
{
	return _vao;
}

