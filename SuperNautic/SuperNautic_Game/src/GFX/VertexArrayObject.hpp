#pragma once

#ifndef VERTEX_ARRAY_OBJECT_HPP
#define VERTEX_ARRAY_OBJECT_HPP

#include <GL/glew.h>
#include <vector>
#include <glm/vec3.hpp>

namespace GFX
{
	//struct Vertex
	//{
	//	glm::vec3 position;
	//	glm::vec3 texCoord;
	//	glm::vec3 normal;
	//};

	class VertexArrayObject
	{
	public:
		VertexArrayObject();
		~VertexArrayObject();

		void addVertexBuffer(GLsizei sizeInBytes, GLenum usage);
		void sendDataToBuffer(GLubyte index, GLuint offset, GLsizei size, GLvoid* data, GLubyte count, GLenum type);

		void bind() const;
		static void unbind();

	protected:
	private:
		class VertexBuffer
		{
		public:
			VertexBuffer(GLsizei sizeInBytes, GLenum usage);
			~VertexBuffer();

			void sendData(GLuint offset, GLsizei size, GLvoid* data);
		protected:
		private:
			GLuint	_vbo;
		};
		
		GLuint	_vao;
		std::vector<VertexBuffer>	_vertexBuffers;

	};

}

#endif //VERTEX_ARRAY_OBJECT
