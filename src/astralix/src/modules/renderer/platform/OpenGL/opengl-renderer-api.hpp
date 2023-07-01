#include "renderer-api.hpp"

namespace astralix {

class OpenGLRendererAPI : public RendererAPI {
public:
  OpenGLRendererAPI() { m_api = API::OpenGL; }

  void init() override;
  void set_viewport(uint32_t x, uint32_t y, uint32_t width,
                    uint32_t height) override;
  void clear_color() override;
  void enable_buffer_testing() override;
  void disable_buffer_testing() override;

  void clear_buffers() override;
};

} // namespace astralix