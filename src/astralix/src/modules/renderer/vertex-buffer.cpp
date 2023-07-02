#include "vertex-buffer.hpp"
#include "engine.hpp"
#include "platform/OpenGL/opengl-vertex-buffer.hpp"

namespace astralix {

Ref<VertexBuffer> VertexBuffer::create(uint32_t size) {
  return create_renderer_component_ref<VertexBuffer, OpenGLVertexBuffer>(
      Engine::get()->renderer_api->get_api(), size);
};

Ref<VertexBuffer> VertexBuffer::create(const void *vertices, uint32_t size) {
  return create_renderer_component_ref<VertexBuffer, OpenGLVertexBuffer>(
      Engine::get()->renderer_api->get_api(), vertices, size);
};

} // namespace astralix
