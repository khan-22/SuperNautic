#include "VertexArrayObject.hpp"

#include "../Log.hpp"

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

void GFX::VertexArrayObject::addIndexBuffer(GLsizei sizeInBytes, GLenum usage)
{
	bind();

	_indexBuffer.reset(new IndexBuffer(sizeInBytes, usage));

	unbind();
}

void GFX::VertexArrayObject::sendDataToBuffer(GLubyte bufferIndex, GLubyte attributeIndex, GLuint offset, GLsizei size, GLvoid* data, GLubyte count, GLenum type)
{
	bind();

	glEnableVertexAttribArray(attributeIndex);
	glVertexAttribPointer(attributeIndex, count, type, GL_FALSE, 0, nullptr);

	_vertexBuffers[bufferIndex].sendData(offset, size, data);
	unbind();
}

void GFX::VertexArrayObject::sendDataToIndexBuffer(GLuint offset, GLsizei size, GLvoid * data)
{
	bind();

	_indexBuffer->sendData(offset, size, data);

	unbind();
}

void VertexArrayObject::setDrawCount(GLuint drawCount)
{
	_drawCount = drawCount;
}

void VertexArrayObject::render()
{
	glDrawElements(GL_TRIANGLES, _drawCount, GL_UNSIGNED_INT, 0);
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
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	LOG_GL_ERRORS();
}

VertexArrayObject::VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &_vbo);
}

void VertexArrayObject::VertexBuffer::sendData(GLuint offset, GLsizei size, GLvoid* data)
{
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);

	LOG_GL_ERRORS();
}

///////////////////////////////////////////////////////////////
////	VertexArrayObject::IndexBuffer
///////////////////////////////////////////////////////////////

GFX::VertexArrayObject::IndexBuffer::IndexBuffer()
{
}

VertexArrayObject::IndexBuffer::IndexBuffer(GLsizei sizeInBytes, GLenum usage)
{
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeInBytes, nullptr, usage);

	LOG_GL_ERRORS();
}

VertexArrayObject::IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &_vbo);
}

void VertexArrayObject::IndexBuffer::sendData(GLuint offset, GLsizei size, GLvoid* data)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);

	LOG_GL_ERRORS();
}
