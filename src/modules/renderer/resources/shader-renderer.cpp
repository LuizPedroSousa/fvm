#include "./shader-renderer.hpp"
#include "./shader.hpp"
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "iostream"
#include <vector>

#include "glad/glad.h"

ShaderRenderer::ShaderRenderer() {
  this->id = glCreateProgram();
  m_uniform = Uniform(id);
}

void ShaderRenderer::use() {
  glUseProgram(this->id);
}

Either<BaseException, Unit> ShaderRenderer::attach_many(Shader *shaders, size_t size) {
  for (int i = 0; i < size; i++) {
    auto attached = this->attach(shaders[i]);
    ASSERT_COMPARE(attached);
  }

  return Unit();
}

Either<BaseException, Unit> ShaderRenderer::attach(Shader shader) {

  glAttachShader(this->id, shader.vertex);
  glAttachShader(this->id, shader.fragment);

  int success;

  glLinkProgram(this->id);

  glGetProgramiv(this->id, GL_LINK_STATUS, &success);

  if (!success) {
    char *infoLog = new char[512];

    glGetProgramInfoLog(this->id, 512, NULL, infoLog);

    ASSERT(true, infoLog);
  };

  glDeleteShader(shader.fragment);
  glDeleteShader(shader.vertex);

  shader.signed_to = this->id;

  this->shaders.push_back(shader);

  return Unit();
}

Either<BaseException, Unit> ShaderRenderer::attach(Either<BaseException, Shader> shader) {
  ASSERT_COMPARE(shader);

  return attach(shader.right());
}