#include "GFX/Resources/VertexArrayObject.hpp"

#include "Core/Io/Log.hpp"

using namespace GFX;

VertexArrayObject::VertexArrayObject() {
  glGenVertexArrays(1, &_vao);

  for (int i = 0; i < 16; i++) {
    _attributeOffsets[i] = 0U;
  }
}

VertexArrayObject::~VertexArrayObject() { glDeleteVertexArrays(1, &_vao); }

void VertexArrayObject::addVertexBuffer(GLsizei sizeInBytes, GLenum usage) {
  bind();
  _vertexBuffers.emplace_back(new VertexBuffer());
  VertexBuffer& buffer = *_vertexBuffers.back();

  buffer.bind();
  buffer.allocate(sizeInBytes, usage);
  unbind();
  buffer.unbind();
}

void GFX::VertexArrayObject::addIndexBuffer(GLsizei sizeInBytes, GLenum usage) {
  bind();

  _indexBuffer.reset(new IndexBuffer());
  _indexBuffer->bind();
  _indexBuffer->allocate(sizeInBytes, usage);
  unbind();
  _indexBuffer->unbind();
}

void GFX::VertexArrayObject::sendDataToBuffer(GLubyte bufferIndex,
                                              GLubyte attributeIndex,
                                              GLuint offset, GLsizei size,
                                              GLvoid* data, GLubyte count,
                                              GLenum type) {
  bind();
  VertexBuffer& buffer = *_vertexBuffers[bufferIndex].get();
  buffer.bind();

  glEnableVertexAttribArray(attributeIndex);

  GLsizei accumulatedOffset = 0U;
  for (int i = 0; i < attributeIndex; i++) {
    accumulatedOffset += _attributeOffsets[i];
  }

  glVertexAttribPointer(attributeIndex, count, type, GL_FALSE, 0,
                        (GLvoid*)(accumulatedOffset));

  // if (attributeIndex == 0)
  //{
  //	glVertexAttribPointer(attributeIndex, count, type, GL_FALSE, 0,
  //nullptr);
  //}
  // else
  //{
  //	glVertexAttribPointer(attributeIndex, count, type, GL_FALSE, 0,
  //(GLvoid*)(_attributeOffsets[attributeIndex-1]));
  //}

  _attributeOffsets[attributeIndex] += size;

  buffer.sendData(offset, size, data);
  unbind();
  buffer.unbind();
}

void GFX::VertexArrayObject::sendDataToIndexBuffer(GLuint offset, GLsizei size,
                                                   GLvoid* data) {
  bind();

  _indexBuffer->bind();
  _indexBuffer->sendData(offset, size, data);
  unbind();
  _indexBuffer->unbind();
}

void VertexArrayObject::setDrawCount(GLuint drawCount) {
  _drawCount = drawCount;
}

void GFX::VertexArrayObject::updateDataInBuffer(GLubyte bufferIndex,
                                                GLubyte attributeIndex,
                                                GLuint offset, GLsizei size,
                                                GLvoid* data) {
  bind();
  VertexBuffer& buffer = *_vertexBuffers[bufferIndex].get();
  buffer.bind();

  buffer.sendData(offset, size, data);

  unbind();
  buffer.unbind();
}

void VertexArrayObject::render() {
  bind();
  glDrawElements(GL_TRIANGLES, _drawCount, GL_UNSIGNED_INT, 0);
  unbind();
}

void GFX::VertexArrayObject::renderPoints() {
  bind();
  glDrawArrays(GL_POINTS, 0, _drawCount);
  unbind();
}

void VertexArrayObject::bind() const { glBindVertexArray(_vao); }

void VertexArrayObject::unbind() { glBindVertexArray(0); }

///////////////////////////////////////////////////////////////
////	VertexArrayObject::VertexBuffer
///////////////////////////////////////////////////////////////

VertexArrayObject::VertexBuffer::VertexBuffer() {
  glGenBuffers(1, &_vbo);
  LOG_GL_ERRORS();
}

VertexArrayObject::VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &_vbo); }

void VertexArrayObject::VertexBuffer::allocate(GLsizei sizeInBytes,
                                               GLenum usage) {
  glBufferData(GL_ARRAY_BUFFER, sizeInBytes, nullptr, usage);
  LOG_GL_ERRORS();
}

void VertexArrayObject::VertexBuffer::bind() {
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
}

void VertexArrayObject::VertexBuffer::unbind() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexArrayObject::VertexBuffer::sendData(GLuint offset, GLsizei size,
                                               GLvoid* data) {
  glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
  LOG_GL_ERRORS();
}

///////////////////////////////////////////////////////////////
////	VertexArrayObject::IndexBuffer
///////////////////////////////////////////////////////////////

GFX::VertexArrayObject::IndexBuffer::IndexBuffer() {
  glGenBuffers(1, &_vbo);
  LOG_GL_ERRORS();
}

void VertexArrayObject::IndexBuffer::allocate(GLsizei sizeInBytes,
                                              GLenum usage) {
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeInBytes, nullptr, usage);
  LOG_GL_ERRORS();
}

void VertexArrayObject::IndexBuffer::bind() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo);
}

void VertexArrayObject::IndexBuffer::unbind() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

VertexArrayObject::IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &_vbo); }

void VertexArrayObject::IndexBuffer::sendData(GLuint offset, GLsizei size,
                                              GLvoid* data) {
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
  LOG_GL_ERRORS();
}
