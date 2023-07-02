#include "renderer-api.hpp"
#include "assert.hpp"
#include "platform/OpenGL/opengl-renderer-api.hpp"

namespace astralix {

Scope<RendererAPI> RendererAPI::create(const API &p_api) {
  return create_renderer_component_scope<RendererAPI, OpenGLRendererAPI>(p_api);
}

} // namespace astralix
