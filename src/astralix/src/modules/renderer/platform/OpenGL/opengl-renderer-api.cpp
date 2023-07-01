#include "opengl-renderer-api.hpp"
#include "glad/glad.h"

namespace astralix {
void OpenGLRendererAPI::init() { enable_buffer_testing(); }

void OpenGLRendererAPI::enable_buffer_testing() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_STENCIL_TEST);
}

void OpenGLRendererAPI::disable_buffer_testing() {
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_STENCIL_TEST);
}

void OpenGLRendererAPI::clear_color() {
  glClearColor(m_clear_color.x, m_clear_color.y, m_clear_color.z,
               m_clear_color.w);
}

void OpenGLRendererAPI::set_viewport(uint32_t x, uint32_t y, uint32_t width,
                                     uint32_t height) {
  glViewport(x, y, width, height);
}

void OpenGLRendererAPI::clear_buffers() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

} // namespace astralix
