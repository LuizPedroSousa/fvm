#include "./shader-renderer.hpp"
#include "./shader.hpp"
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "game.hpp"
#include <vector>

#include "glad/glad.h"

ShaderRenderer::ShaderRenderer() {
  this->id = glCreateProgram();
  m_uniform = Uniform(id);
}

void ShaderRenderer::use() {
  glUseProgram(this->id);
}

Either<BaseException, Unit> ShaderRenderer::attach_many(ResourceID *shader_ids, size_t size) {
  for (int i = 0; i < size; i++) {
    this->attach(shader_ids[i]);
  }
}

Either<BaseException, Unit> ShaderRenderer::attach(ResourceID p_id) {
  auto resource_manager = Game::get()->get_resource_manager();
  auto shader_ptr = resource_manager->get_shader_by_id(p_id);

  ASSERT(shader_ptr == nullptr, "Shader not found");

  glAttachShader(this->id, shader_ptr->vertex);
  glAttachShader(this->id, shader_ptr->fragment);

  int success;

  glLinkProgram(this->id);

  glGetProgramiv(this->id, GL_LINK_STATUS, &success);

  if (!success) {
    char *infoLog = new char[512];

    glGetProgramInfoLog(this->id, 512, NULL, infoLog);

    ASSERT(true, infoLog);
  };

  glDeleteShader(shader_ptr->fragment);
  glDeleteShader(shader_ptr->vertex);

  shader_ptr->signed_to = this->id;

  m_shaders.push_back(p_id);

  return Unit();
}
