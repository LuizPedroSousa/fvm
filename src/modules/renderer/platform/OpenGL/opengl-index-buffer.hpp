#pragma once
#include "index-buffer.hpp"

namespace astralix {
class OpenGLIndexBuffer : public IndexBuffer {
public:
  OpenGLIndexBuffer(uint32_t *indices, uint32_t count);
  ~OpenGLIndexBuffer();

  void bind() const override;
  void unbind() const override;

  void set_data(const void *data, uint32_t size) const override;

  uint32_t get_count() const override { return m_count; };

private:
  uint32_t m_renderer_id;

  uint32_t m_count;
};

} // namespace astralix
