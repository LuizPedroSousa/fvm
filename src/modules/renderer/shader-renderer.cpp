#include "glad/glad.h"
#include "./shader-renderer.hpp"
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include <vector>
#include "./shader.hpp"

ShaderRenderer::ShaderRenderer()
{
  this->id = glCreateProgram();
}

void ShaderRenderer::use()
{
  glUseProgram(this->id);
}

Either<BaseException, Unit> ShaderRenderer::attachMany(Shader *shaders, size_t size)
{
  for (int i = 0; i < size; i++)
  {
    auto attached = this->attach(shaders[i]);

    if (attached.isLeft())
    {
      return attached.left();
    }
  }

  return Unit();
}

Either<BaseException, Unit> ShaderRenderer::attach(Shader shader)
{
  glAttachShader(this->id, shader.vertex);
  glAttachShader(this->id, shader.fragment);

  int success;

  glLinkProgram(this->id);

  glGetProgramiv(this->id, GL_LINK_STATUS, &success);

  if (!success)
  {
    char infoLog[512];

    glGetProgramInfoLog(this->id, 512, NULL, infoLog);

    return BaseException(infoLog);
  };

  glDeleteShader(shader.fragment);
  glDeleteShader(shader.vertex);

  shader.signed_to = this->id;

  this->shaders.push_back(shader);

  return Unit();
}