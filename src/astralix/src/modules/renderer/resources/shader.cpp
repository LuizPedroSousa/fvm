#include "shader.hpp"
#include "filesystem"
#include "fstream"
#include "glad/glad.h"
#include "iostream"
#include "sstream"
#include <cstring>

namespace astralix {

Shader::Shader() {}

Shader::Shader(RESOURCE_INIT_PARAMS, unsigned int vertex,
               const char *vertex_filename, unsigned int fragment,
               const char *fragment_filename)
    : RESOURCE_INIT(), vertex(vertex), vertex_filename(vertex_filename),
      fragment(fragment), fragment_filename(fragment_filename) {}

Either<BaseException, Shader> Shader::create(ResourceID id,
                                             const char *vertex_filename,
                                             const char *fragment_filename) {
  auto vertex = compile(vertex_filename, GL_VERTEX_SHADER);

  ASSERT_COMPARE(vertex);

  auto fragment = compile(fragment_filename, GL_FRAGMENT_SHADER);

  ASSERT_COMPARE(fragment);

  return Shader(id, vertex.right(), vertex_filename, fragment.right(),
                fragment_filename);
}

Either<BaseException, Shader *>
create_many(std::tuple<ResourceID, const char *, const char *> files[],
            size_t size) {
  Shader shaders[size * 2];

  for (int i = 0; i <= size - 1; i++) {
    auto shader = Shader::create(std::get<0>(files[i]), std::get<1>(files[i]),
                                 std::get<2>(files[i]));

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

    ASSERT(true, error);
  };

  return shader;
};

} // namespace astralix