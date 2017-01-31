#pragma once

#ifndef VERTEX_ARRAY_OBJECT_HPP
#define VERTEX_ARRAY_OBJECT_HPP

#include <GL/glew.h>
#include <vector>
#include <glm/vec3.hpp>
#include <memory>

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
		void addIndexBuffer(GLsizei sizeInBytes, GLenum usage);
		void sendDataToBuffer(GLubyte bufferIndex, GLubyte attributeIndex, GLuint offset, GLsizei size, GLvoid* data, GLubyte count, GLenum type);
		void sendDataToIndexBuffer(GLuint offset, GLsizei size, GLvoid* data);
		void setDrawCount(GLuint drawCount);

		void render();

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
		
		class IndexBuffer
		{
		public:
			IndexBuffer();
			IndexBuffer(GLsizei sizeInBytes, GLenum usage);
			~IndexBuffer();

			void sendData(GLuint offset, GLsizei size, GLvoid* data);
		protected:
		private:
			GLuint	_vbo;
		};

		GLuint	_vao;
		GLuint	_drawCount;

		// INDEX BUFFER UNIQUE POINTER FÅR INTE KOPIERAS.
		// Kopieras det någonstans??

		std::vector<VertexBuffer>	 _vertexBuffers;
		//std::unique_ptr<IndexBuffer> _indexBuffer;

	};

}

#endif //VERTEX_ARRAY_OBJECT
