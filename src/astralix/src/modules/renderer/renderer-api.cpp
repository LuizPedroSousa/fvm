#include "renderer-api.hpp"
#include "assert.hpp"
#include "platform/OpenGL/opengl-renderer-api.hpp"

namespace astralix {

Scope<RendererAPI> RendererAPI::create(const API &p_api) {
  switch (p_api) {
  case API::OpenGL:
    return create_scope<OpenGLRendererAPI>();

  default:
    ASTRA_ASSERT_THROW(true, "NONE isn't a valid renderer API type");
  }
}

} // namespace astralix
