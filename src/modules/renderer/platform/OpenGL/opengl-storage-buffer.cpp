#include "opengl-storage-buffer.hpp"
#include "glad/glad.h"

namespace astralix {

OpenGLStorageBuffer::OpenGLStorageBuffer(uint32_t size) : m_size(size) {
  glGenBuffers(1, &m_renderer_id);

  bind();

  glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

  bind_base(0);

  unbind();
}

OpenGLStorageBuffer::~OpenGLStorageBuffer() {
  glDeleteBuffers(1, &m_renderer_id);
}

void OpenGLStorageBuffer::bind() const {
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_renderer_id);
}

void OpenGLStorageBuffer::bind_base(uint32_t point) const {
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, point, m_renderer_id);
}

void OpenGLStorageBuffer::set_data(const void *data, uint32_t size) const {
  bind();
  glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, data);
}

void OpenGLStorageBuffer::unbind() const {
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

} // namespace astralix
