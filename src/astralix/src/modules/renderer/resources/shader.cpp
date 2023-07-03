#include "shader.hpp"
#include "assert.hpp"
#include "engine.hpp"
#include "filesystem"
#include "fstream"
#include "glad/glad.h"
#include "iostream"
#include "optional"
#include "platform/OpenGL/opengl-shader.hpp"
#include "sstream"
#include <cstring>

namespace astralix {

std::string Shader::get_file(const std::string &path) {
  auto absolute_path = std::filesystem::current_path()
                           .parent_path()
                           .append("src")
                           .append("assets")
                           .append("shaders")
                           .append(path);

  bool file_exists = std::filesystem::exists(absolute_path);

  ASTRA_ASSERT_THROW(!file_exists,
                     "File" + std::string(absolute_path) + " doesn't exists");

  std::string result;

  std::ifstream in(absolute_path,
                   std::ios::in |
                       std::ios::binary); // ifstream closes itself due to RAII
  if (in) {
    in.seekg(0, std::ios::end);
    size_t size = in.tellg();
    if (size != -1) {
      result.resize(size);
      in.seekg(0, std::ios::beg);
      in.read(&result[0], size);
    } else {
      ASTRA_ASSERT_THROW(true, "Could not read from file");
    }
  } else {
    ASTRA_ASSERT_THROW(true,
                       "Can't open file" + std::string(absolute_path) + "");
  }

  return result;
}

Ref<Shader> Shader::create(const ResourceID &resource_id,
                           const std::string &fragment_path,
                           const std::string &vertex_path,
                           const std::string &geometry_path) {
  return create_renderer_component_ref<Shader, OpenGLShader>(
      Engine::get()->renderer_api->get_api(), resource_id, fragment_path,
      vertex_path, geometry_path);
}

} // namespace astralix