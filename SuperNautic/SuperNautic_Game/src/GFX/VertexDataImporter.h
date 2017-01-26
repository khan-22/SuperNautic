#pragma once

#ifndef VERTEX_DATA_IMPORTER_H
#define VERTEX_DATA_IMPORTER_H

#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace GFX {

	struct RawVertexData 
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> texCoords;
		std::vector<glm::vec3> normals;
	};

	struct RawMeshCollection
	{
		std::vector<RawVertexData> meshes;
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
#endif //VERTEX_DATA_IMPORTER_H