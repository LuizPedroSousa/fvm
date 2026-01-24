#include "shader.hpp"
#include "assert.hpp"
#include "engine.hpp"
#include "filesystem"
#include "fstream"
#include "glad/glad.h"
#include "iostream"
#include "platform/OpenGL/opengl-shader.hpp"
#include <cstring>

namespace astralix {

Ref<Shader> Shader::create(const ResourceID &resource_id,
                           Ref<Path> fragment_path, Ref<Path> vertex_path,
                           Ref<Path> geometry_path) {
  return create_renderer_component_ref<Shader, OpenGLShader>(
      Engine::get()->renderer_api->get_api(), resource_id, fragment_path,
      vertex_path, geometry_path);
}

} // namespace astralix
