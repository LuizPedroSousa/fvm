#pragma once

#include "assert.hpp"
#include "base.hpp"
#include "glm/glm.hpp"
#include "vertex-array.hpp"

namespace astralix {

class RendererAPI {

public:
  enum class API { None = 0, OpenGL = 1 };
  enum DrawPrimitiveType { POINTS = 0, LINES = 1, TRIANGLES = 2 };

  virtual void init() = 0;
  virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width,
                            uint32_t height) = 0;
  virtual void clear_color() = 0;
  virtual void clear_buffers() = 0;
  virtual void disable_buffer_testing() = 0;
  virtual void enable_buffer_testing() = 0;
  virtual void
  draw_indexed(const Ref<VertexArray> &vertex_array,
               DrawPrimitiveType primitive_type = DrawPrimitiveType::TRIANGLES,
               uint32_t index_count = -1) = 0;

  virtual void draw_lines(const Ref<VertexArray> &vertex_array,
                          uint32_t vertex_count) = 0;

  API get_api() { return m_api; };

  void set_clear_color(const glm::vec4 &p_clear_color) {
    m_clear_color = p_clear_color;
  };

  static Scope<RendererAPI> create(const API &p_api);

protected:
  glm::vec4 m_clear_color = glm::vec4(0.5f, 0.5f, 1.0f, 0.0f);
  API m_api;
};

template <typename T, typename O, typename... Args>
Ref<T> create_renderer_component_ref(RendererAPI::API api, Args &&...params) {
  switch (api) {
  case RendererAPI::API::OpenGL:
    return create_ref<O>(std::forward<Args>(params)...);

  default:
    ASTRA_ASSERT_THROW(true, "NONE ins't a valid renderer api");
  }
}

template <typename T, typename O, typename... Args>
Scope<T> create_renderer_component_scope(RendererAPI::API api,
                                         Args &&...params) {
  switch (api) {
  case RendererAPI::API::OpenGL:
    return create_scope<O>(std::forward<Args>(params)...);

  default:
    ASTRA_ASSERT_THROW(true, "NONE ins't a valid renderer api");
  }
}

} // namespace astralix
