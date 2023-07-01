#include "shader.hpp"
#include "assert.hpp"
#include "filesystem"
#include "fstream"
#include "glad/glad.h"
#include "iostream"
#include "optional"
#include "sstream"
#include <cstring>

namespace astralix {

Shader::Shader() {}

Shader::Shader(RESOURCE_INIT_PARAMS, unsigned int vertex,
               const char *vertex_filename, unsigned int fragment,
               const char *fragment_filename)
    : RESOURCE_INIT(), vertex(vertex), m_vertex_filename(vertex_filename),
      fragment(fragment), m_fragment_filename(fragment_filename) {}

Shader::Shader(RESOURCE_INIT_PARAMS, unsigned int vertex,
               const char *vertex_filename, unsigned int fragment,
               const char *fragment_filename, unsigned int geometry,
               const char *geometry_filename)
    : Shader(id, vertex, vertex_filename, fragment, fragment_filename) {
  this->geometry = geometry;
  this->m_geometry_filename = geometry_filename;
}

Either<BaseException, Shader> Shader::create(CreateShaderDTO dto) {
  auto vertex = compile(dto.vertex_filename, GL_VERTEX_SHADER);

  ASTRA_ASSERT_EITHER(vertex);

  auto fragment = compile(dto.fragment_filename, GL_FRAGMENT_SHADER);

  ASTRA_ASSERT_EITHER(fragment);

  if (dto.geometry_filename != NULL) {
    auto geometry = compile(dto.geometry_filename, GL_GEOMETRY_SHADER);

    ASTRA_ASSERT_EITHER(geometry);

    return Shader(dto.id, vertex.right(), dto.vertex_filename, fragment.right(),
                  dto.fragment_filename, geometry.right(),
                  dto.geometry_filename);
  }

  return Shader(dto.id, vertex.right(), dto.vertex_filename, fragment.right(),
                dto.fragment_filename);
}

Either<BaseException, Shader *> create_many(CreateShaderDTO dtos[],
                                            size_t size) {
  Shader shaders[size * 2];

  for (int i = 0; i <= size - 1; i++) {
    auto shader = Shader::create(dtos[i]);

    if (shader.isLeft()) {
      return shader.left();
    }

    shaders[i] = shader.right();
  }

  return shaders;
};

const char *Shader::get_file(const char *filename) {
  auto path = std::filesystem::current_path()
                  .parent_path()
                  .append("src")
                  .append("assets")
                  .append("shaders")
                  .append(filename);

  std::ifstream file(path);
  std::stringstream buffer;
  buffer << file.rdbuf();

  std::string bufferstr = buffer.str();

  char *charPtr = new char[bufferstr.length() + 1];
  std::strcpy(charPtr, bufferstr.c_str());
  return charPtr;
}

Either<BaseException, u_int> Shader::compile(const char *filename, int type) {
  unsigned int shader;

  const char *shader_source = get_file(filename);

  shader = glCreateShader(type);

  glShaderSource(shader, 1, &shader_source, NULL);

  glCompileShader(shader);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    char *shader_error = new char[512];
    glGetShaderInfoLog(shader, sizeof(shader_error), NULL, shader_error);

    std::string error = "Can't load shader " + std::string(filename) + "\n" +
                        std::string(shader_error);

    return BaseException(__FILE__, __LINE__, error);
  };

  return shader;
};

} // namespace astralix