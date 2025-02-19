#include "vertex-buffer.hpp"

namespace astralix {

class OpenGLVertexBuffer : public VertexBuffer {
public:
  OpenGLVertexBuffer(const void *vertices, uint32_t size, DrawType draw_type);
  OpenGLVertexBuffer(uint32_t size);
  ~OpenGLVertexBuffer();

  void bind() const override;
  void unbind() const override;

  void set_data(const void *data, uint32_t size) override;

  const BufferLayout &get_layout() const override { return m_layout; };

  void set_layout(const BufferLayout &layout) override { m_layout = layout; };

private:
  uint32_t m_renderer_id;
  BufferLayout m_layout;

  int drawTypeToGL(DrawType draw_type);
};

} // namespace astralix
