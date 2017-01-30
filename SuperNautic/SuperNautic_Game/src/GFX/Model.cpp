#include "Model.hpp"

using namespace GFX;


Model::Model()
{
}


Model::~Model()
{
}

Model::Mesh& GFX::Model::addMesh()
{
	_meshes.emplace_back();
	return _meshes.back();
}


///////////////////////////////////////////////////////////////
////	Model::Mesh
///////////////////////////////////////////////////////////////

GFX::Model::Mesh::Mesh()
{
}

GFX::Model::Mesh::~Mesh()
{
}

VertexArrayObject& GFX::Model::Mesh::getVertexArrayObject()
{
	return _vao;
}

