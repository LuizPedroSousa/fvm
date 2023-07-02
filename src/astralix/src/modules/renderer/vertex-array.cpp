#include "vertex-array.hpp"
#include "engine.hpp"
#include "platform/OpenGL/opengl-vertex-array.hpp"

namespace astralix {
Ref<VertexArray> VertexArray::create() {
  return create_renderer_component_ref<VertexArray, OpenGLVertexArray>(
      Engine::get()->renderer_api->get_api());
}
} // namespace astralix
