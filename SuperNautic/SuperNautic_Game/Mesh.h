#pragma once

#ifndef MESH_H
#define MESH_H

#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoord;
	glm::vec3 normal;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

protected:

private:


};


#endif //MESH_H
