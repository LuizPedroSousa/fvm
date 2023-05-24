#include "./shader.hpp"
#include "filesystem"
#include "fstream"
#include "glad/glad.h"
#include "iostream"
#include "sstream"
#include <cstring>

Shader::Shader() {
}

Shader::Shader(unsigned int vertex, unsigned int fragment) : vertex(vertex), fragment(fragment) {
}

Either<BaseException, Shader> Shader::create(const char *vertex_filename, const char *fragment_filename) {
  auto vertex = compile(vertex_filename, GL_VERTEX_SHADER);

  if (vertex.isLeft()) {
    return vertex.left();
  }

  auto fragment = compile(fragment_filename, GL_FRAGMENT_SHADER);

  if (fragment.isLeft()) {
    return fragment.left();
  }

  return Shader(vertex.right(), fragment.right());
}

Either<BaseException, Shader *> Shader::createMany(std::pair<const char *, const char *> files[], size_t size) {
  Shader shaders[size * 2];

  for (int i = 0; i <= size - 1; i++) {
    auto shader = Shader::create(files[i].first, files[i].second);

    if (shader.isLeft()) {
      return shader.left();
    }

    shaders[i] = shader.right();
  }

  return shaders;
};

const char *Shader::getFile(const char *filename) {
  auto path = std::filesystem::current_path().parent_path().append("src/assets/shaders").append(filename);

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

  const char *shader_source = getFile(filename);

  shader = glCreateShader(type);

  glShaderSource(shader, 1, &shader_source, NULL);

  glCompileShader(shader);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    char *infoLog = new char[512];
    glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
    ASSERT(true, infoLog);
  };

  return shader;
};