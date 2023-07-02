#include "framebuffer.hpp"
#include "assert.h"
#include "engine.hpp"
#include "platform/OpenGL/opengl-framebuffer.hpp"

namespace astralix {

Ref<Framebuffer> Framebuffer::create(const FramebufferSpecification &spec) {
  return create_renderer_component_ref<Framebuffer, OpenGLFramebuffer>(
      Engine::get()->renderer_api->get_api(), spec);
};

} // namespace astralix
