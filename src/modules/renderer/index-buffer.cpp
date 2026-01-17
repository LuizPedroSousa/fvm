#include "index-buffer.hpp"

#include "engine.hpp"
#include "platform/OpenGL/opengl-index-buffer.hpp"

namespace astralix {

Ref<IndexBuffer> IndexBuffer::create(u_int32_t *indices, u_int32_t count) {
  return create_renderer_component_ref<IndexBuffer, OpenGLIndexBuffer>(
      Engine::get()->renderer_api->get_api(), indices, count);
}

} // namespace astralix
