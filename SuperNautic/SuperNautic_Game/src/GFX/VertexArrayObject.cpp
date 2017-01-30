#include "VertexArrayObject.hpp"

using namespace GFX;

VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &_vao);
}

VertexArrayObject::~VertexArrayObject()
{
	glDeleteVertexArrays(1, &_vao);
}

void VertexArrayObject::addVertexBuffer(GLsizei sizeInBytes, GLenum usage)
{
	bind();

	_vertexBuffers.emplace_back(sizeInBytes, usage);

	unbind();
}

void GFX::VertexArrayObject::sendDataToBuffer(GLubyte index, GLuint offset, GLsizei size, GLvoid* data, GLubyte count, GLenum type)
{
	bind();

	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, count, type, GL_FALSE, 0, nullptr);

	_vertexBuffers[index].sendData(offset, size, data);
	unbind();
}

void VertexArrayObject::bind() const
{
	glBindVertexArray(_vao);
}

void VertexArrayObject::unbind()
{
	glBindVertexArray(0);
}

///////////////////////////////////////////////////////////////
////	VertexArrayObject::VertexBuffer
///////////////////////////////////////////////////////////////

VertexArrayObject::VertexBuffer::VertexBuffer(GLsizei sizeInBytes, GLenum usage)
{
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeInBytes, nullptr, usage);
}

VertexArrayObject::VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &_vbo);
}

void GFX::VertexArrayObject::VertexBuffer::sendData(GLuint offset, GLsizei size, GLvoid * data)
{
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);

}



