#pragma once

#ifndef VERTEX_DATA_IMPORTER_HPP
#define VERTEX_DATA_IMPORTER_HPP

#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <GL/glew.h>

namespace GFX {

	struct RawVertexData 
	{
		std::string	 name;
		unsigned int textureIndex;

		std::vector<glm::vec3>		vertices;
		std::vector<glm::vec3>		texCoords;
		std::vector<glm::vec3>		normals;
		std::vector<glm::uvec3>		faces;
		std::vector<GLuint>			indices;
		GLuint						largestIndex;
	};

	struct RawMeshCollection
	{
		std::vector<RawVertexData>	meshes;
		std::vector<glm::mat4>		cameras;

	};

	class VertexDataImporter
	{
	public:
		VertexDataImporter(const std::string& rootPath);
		virtual ~VertexDataImporter();

		RawMeshCollection* importVertexData(std::string filepath);

	protected:
		std::string _rootPath;

	private:
	};

}
#endif //VERTEX_DATA_IMPORTER_HPP