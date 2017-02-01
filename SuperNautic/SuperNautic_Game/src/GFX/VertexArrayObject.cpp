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

	_vertexBuffers.emplace_back();
    _vertexBuffers.back().bind();
    _vertexBuffers.back().allocate(sizeInBytes, usage);
	unbind();
    _vertexBuffers.back().unbind();

}

void GFX::VertexArrayObject::addIndexBuffer(GLsizei sizeInBytes, GLenum usage)
{
	bind();

	_indexBuffer.reset(new IndexBuffer());
	_indexBuffer->bind();
    _indexBuffer->allocate(sizeInBytes, usage);
	unbind();
	_indexBuffer->unbind();
}

void GFX::VertexArrayObject::sendDataToBuffer(GLubyte bufferIndex, GLubyte attributeIndex, GLuint offset, GLsizei size, GLvoid* data, GLubyte count, GLenum type)
{
	bind();
	VertexBuffer& buffer = _vertexBuffers[bufferIndex];
	buffer.bind();


	glEnableVertexAttribArray(attributeIndex);
	glVertexAttribPointer(attributeIndex, count, type, GL_FALSE, 0, nullptr);

	buffer.sendData(offset, size, data);
	unbind();
	buffer.unbind();
}

void GFX::VertexArrayObject::sendDataToIndexBuffer(GLuint offset, GLsizei size, GLvoid * data)
{
	bind();

	_indexBuffer->bind();
	_indexBuffer->sendData(offset, size, data);
	unbind();
	_indexBuffer->unbind();
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

VertexArrayObject::VertexBuffer::VertexBuffer()
{
	glGenBuffers(1, &_vbo);
	LOG_GL_ERRORS();
}

VertexArrayObject::VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &_vbo);
}

void VertexArrayObject::VertexBuffer::allocate(GLsizei sizeInBytes, GLenum usage)
{
	glBufferData(GL_ARRAY_BUFFER, sizeInBytes, nullptr, usage);
	LOG_GL_ERRORS();
}

void VertexArrayObject::VertexBuffer::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
}

void VertexArrayObject::VertexBuffer::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexArrayObject::VertexBuffer::sendData(GLuint offset, GLsizei size, GLvoid* data)
{
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	LOG_GL_ERRORS();
}

///////////////////////////////////////////////////////////////
////	VertexArrayObject::IndexBuffer
///////////////////////////////////////////////////////////////

GFX::VertexArrayObject::IndexBuffer::IndexBuffer()
{
	glGenBuffers(1, &_vbo);
	LOG_GL_ERRORS();
}

void VertexArrayObject::IndexBuffer::allocate(GLsizei sizeInBytes, GLenum usage)
{
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeInBytes, nullptr, usage);
	LOG_GL_ERRORS();
}


void VertexArrayObject::IndexBuffer::bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo);
}

void VertexArrayObject::IndexBuffer::unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

VertexArrayObject::IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &_vbo);
}

void VertexArrayObject::IndexBuffer::sendData(GLuint offset, GLsizei size, GLvoid* data)
{
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
	LOG_GL_ERRORS();
}
