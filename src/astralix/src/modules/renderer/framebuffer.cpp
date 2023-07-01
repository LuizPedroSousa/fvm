#include "framebuffer.hpp"
#include "assert.h"
#include "engine.hpp"
#include "platform/OpenGL/opengl-framebuffer.hpp"

namespace astralix {

Ref<Framebuffer> Framebuffer::create(const FramebufferSpecification &spec) {
  switch (Engine::get()->renderer_api->get_api()) {
  case RendererAPI::API::OpenGL:
    return create_ref<OpenGLFramebuffer>(spec);
  default:
    ASTRA_ASSERT_THROW(true, "NONE ins't a valid renderer api");
    break;
  }
};

} // namespace astralix
