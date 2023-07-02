#include "opengl-vertex-buffer.hpp"
#include "glad/glad.h"

namespace astralix {

OpenGLVertexBuffer::OpenGLVertexBuffer(const void *vertices, uint32_t size) {
  glGenBuffers(1, &m_renderer_id);

  bind();

  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
};

OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
  bind();

  glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
};

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
  glDeleteBuffers(1, &m_renderer_id);
};

void OpenGLVertexBuffer::bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
}

void OpenGLVertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void OpenGLVertexBuffer::set_data(const void *data, uint32_t size) {
  bind();
  glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

} // namespace astralix
