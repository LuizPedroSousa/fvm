
#pragma once

#include "assert.hpp"
#include "base.hpp"
#include "vector"

namespace astralix {

  enum class ShaderDataType {
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
  };

  static uint32_t shader_data_type_size(ShaderDataType type) {
    switch (type) {
    case ShaderDataType::Float:
      return 4;
    case ShaderDataType::Float2:
      return 4 * 2;
    case ShaderDataType::Float3:
      return 4 * 3;
    case ShaderDataType::Float4:
      return 4 * 4;
    case ShaderDataType::Mat3:
      return 4 * 3 * 3;
    case ShaderDataType::Mat4:
      return 4 * 4 * 4;
    case ShaderDataType::Int:
      return 4;
    case ShaderDataType::Int2:
      return 4 * 2;
    case ShaderDataType::Int3:
      return 4 * 3;
    case ShaderDataType::Int4:
      return 4 * 4;
    case ShaderDataType::Bool:
      return 1;
    }

    ASTRA_EXCEPTION(true, "Unknown ShaderDataType!");
    return 0;
  }

  struct BufferElement {
    std::string name;
    ShaderDataType type;
    uint32_t size;
    size_t offset;
    bool normalized;

    BufferElement() = default;

    BufferElement(ShaderDataType type, const std::string& name,
      bool normalized = false)
      : name(name), type(type), size(shader_data_type_size(type)), offset(0),
      normalized(normalized) {    
}

    uint32_t get_component_count() const {
      switch (type) {
      case ShaderDataType::Float:
        return 1;
      case ShaderDataType::Float2:
        return 2;
      case ShaderDataType::Float3:
        return 3;
      case ShaderDataType::Float4:
        return 4;
      case ShaderDataType::Mat3:
        return 3; // 3* float3
      case ShaderDataType::Mat4:
        return 4; // 4* float4
      case ShaderDataType::Int:
        return 1;
      case ShaderDataType::Int2:
        return 2;
      case ShaderDataType::Int3:
        return 3;
      case ShaderDataType::Int4:
        return 4;
      case ShaderDataType::Bool:
        return 1;
      }

      ASTRA_EXCEPTION(false, "Unknown ShaderDataType!");
      return 0;
    }
  };

  class BufferLayout {
  public:
    BufferLayout() {}

    BufferLayout(std::initializer_list<BufferElement> elements)
      : m_elements(elements) {
      calculate_offsets_and_stride();
    }

    uint32_t get_stride() const { return m_stride; }
    const std::vector<BufferElement>& get_elements() const { return m_elements; }

    std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
    std::vector<BufferElement>::iterator end() { return m_elements.end(); }
    std::vector<BufferElement>::const_iterator begin() const {
      return m_elements.begin();
    }
    std::vector<BufferElement>::const_iterator end() const {
      return m_elements.end();
    }

  private:
    void calculate_offsets_and_stride() {
      size_t offset = 0;
      m_stride = 0;
      for (auto& element : m_elements) {
        element.offset = offset;
        offset += element.size;
        m_stride += element.size;
      }
    }

  private:
    std::vector<BufferElement> m_elements;
    uint32_t m_stride = 0;
  };

  class VertexBuffer {

  public:
    enum DrawType { Static = 0, Dynamic = 1 };
    virtual ~VertexBuffer() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void set_data(const void* data, uint32_t size) = 0;

    virtual const BufferLayout& get_layout() const = 0;
    virtual void set_layout(const BufferLayout& layout) = 0;

    static Ref<VertexBuffer> create(uint32_t size);
    static Ref<VertexBuffer> create(const void* vertices, uint32_t size,
      DrawType draw_type);
  };

} // namespace astralix
