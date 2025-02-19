#include "opengl-vertex-array.hpp"
#include "assert.hpp"
#include "glad/glad.h"

namespace astralix {
  GLenum OpenGLVertexArray::map_shader_data_type_to_opengl(ShaderDataType type) {
    switch (type) {
    case ShaderDataType::Float:
      return GL_FLOAT;
    case ShaderDataType::Float2:
      return GL_FLOAT;
    case ShaderDataType::Float3:
      return GL_FLOAT;
    case ShaderDataType::Float4:
      return GL_FLOAT;
    case ShaderDataType::Mat3:
      return GL_FLOAT;
    case ShaderDataType::Mat4:
      return GL_FLOAT;
    case ShaderDataType::Int:
      return GL_INT;
    case ShaderDataType::Int2:
      return GL_INT;
    case ShaderDataType::Int3:
      return GL_INT;
    case ShaderDataType::Int4:
      return GL_INT;
    case ShaderDataType::Bool:
      return GL_BOOL;
    }

    ASTRA_EXCEPTION(true, "Unknown shader data type");
  }

  const void* OpenGLVertexArray::get_offset(bool is_matrix,
    BufferElement element) {

    if (is_matrix) {
      uint8_t count = element.get_component_count();

      for (uint8_t i = 0; i < count; i++) {
        return (const void*)(element.offset + sizeof(float) * count * i);
      }
    }

    return (const void*)element.offset;
  }

  OpenGLVertexArray::OpenGLVertexArray() { glGenVertexArrays(1, &m_renderer_id); }
  OpenGLVertexArray::~OpenGLVertexArray() {
    glDeleteVertexArrays(1, &m_renderer_id);
  }

  void OpenGLVertexArray::bind() const { glBindVertexArray(m_renderer_id); }
  void OpenGLVertexArray::unbind() const { glBindVertexArray(0); }

  void OpenGLVertexArray::add_vertex_buffer(
    const Ref<VertexBuffer>& vertex_buffer) {

    const auto& layout = vertex_buffer->get_layout();

    ASTRA_EXCEPTION(layout.get_elements().size() == 0,
      "Vertex Buffer has no layout elements!")

      bind();

    for (const auto& element : layout.get_elements()) {
      glEnableVertexAttribArray(m_vertex_buffer_index);
      bool is_matrix = element.type == ShaderDataType::Mat4 ||
        element.type == ShaderDataType::Mat3;

      glVertexAttribPointer(m_vertex_buffer_index, element.get_component_count(),
        map_shader_data_type_to_opengl(element.type),
        element.normalized ? GL_TRUE : GL_FALSE,
        layout.get_stride(), get_offset(is_matrix, element));

      if (is_matrix) {
        glVertexAttribDivisor(m_vertex_buffer_index, 1);
      }

      m_vertex_buffer_index++;
    }
    m_vertex_buffers.push_back(vertex_buffer);
  }

  void OpenGLVertexArray::set_index_buffer(const Ref<IndexBuffer>& index_buffer) {
    bind();
    index_buffer->bind();

    m_index_buffer = index_buffer;
  }

} // namespace astralix
