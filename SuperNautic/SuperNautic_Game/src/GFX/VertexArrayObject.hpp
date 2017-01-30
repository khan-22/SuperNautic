#pragma once

#ifndef VERTEX_ARRAY_OBJECT_HPP
#define VERTEX_ARRAY_OBJECT_HPP

#include <GL/glew.h>
#include <vector>
#include <glm/vec3.hpp>

namespace GFX
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 texCoord;
		glm::vec3 normal;
	};

	//class VertexArrayObject
	//{
	//public:
	//	VertexArrayObject();
	//	~VertexArrayObject();

	//	template <typename dataT>
	//	bool addVertexBuffer();

	//	void bind() const;
	//	static void unbind();

	//protected:
	//private:
	//	template <typename dataT>
	//	class VertexBuffer
	//	{
	//	public:
	//		VertexBuffer(std::vector<dataT>& data);
	//		~VertexBuffer();
	//	protected:
	//	private:
	//		GLuint	_vbo;
	//	};
	//	
	//	GLuint	_vao;
	//	std::vector<VertexBuffer>	_vertexBuffers;

	//};

	//template <typename dataT>
	//bool VertexArrayObject::addVertexBuffer()
	//{
	//	bind();

	//	_vertexBuffers.emplace_back();
	//	VertexBuffer& vertexBuffer = _vertexBuffers.back();

	//}

	//template <typename dataT>
	//VertexArrayObject::VertexBuffer<dataT>::VertexBuffer(std::vector<dataT>& data)
	//{
	//	glGenBuffers(1, &_vbo);
	//	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	//	glBufferData(GL_ARRAY_BUFFER, )


	//}
}

#endif //VERTEX_ARRAY_OBJECT
