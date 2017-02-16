#include "fileOutputFuncs.h"

void writeHeader(Header* header, FILE * file)
{
	fwrite(header, sizeof(Header), 1, file);
}

void writeHeader(MeshHeader* header, FILE * file)
{
	fwrite(header, sizeof(MeshHeader), 1, file);
}

void writeData(Mesh* mesh, FILE * file)
{
	fwrite(mesh->name.data(), sizeof(mesh->name[0]), mesh->name.size(), file);
	fwrite(mesh->positions.data(), sizeof(mesh->positions[0]), mesh->positions.size(), file);
	fwrite(mesh->texCoords.data(), sizeof(mesh->texCoords[0]), mesh->texCoords.size(), file);
	fwrite(mesh->normals.data(), sizeof(mesh->normals[0]), mesh->normals.size(), file);
	fwrite(mesh->faces.data(), sizeof(mesh->faces[0]), mesh->faces.size(), file);
}

void writeData(Light* light, FILE * file)
{
	fwrite(light, sizeof(Light), 1, file);
}

void writeData(Camera* camera, FILE * file)
{
	fwrite(&camera->transform[0][0], sizeof(camera->transform), 1, file);
}
