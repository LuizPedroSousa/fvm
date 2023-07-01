#pragma once
#include "base.hpp"
#include "glm/glm.hpp"

namespace astralix {

class RendererAPI {

public:
  enum class API { None = 0, OpenGL = 1 };

  virtual void init() = 0;
  virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width,
                            uint32_t height) = 0;
  virtual void clear_color() = 0;
  virtual void clear_buffers() = 0;
  virtual void disable_buffer_testing() = 0;
  virtual void enable_buffer_testing() = 0;

  API get_api() { return m_api; };

  void set_clear_color(const glm::vec4 &p_clear_color) {
    m_clear_color = p_clear_color;
  };

  static Scope<RendererAPI> create(const API &p_api);

protected:
  glm::vec4 m_clear_color = glm::vec4(0.5f, 0.5f, 1.0f, 0.0f);
  API m_api;
};

} // namespace astralix
