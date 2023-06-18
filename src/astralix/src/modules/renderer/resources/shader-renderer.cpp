#include "shader-renderer.hpp"
#include "either.hpp"
#include "engine.hpp"
#include "exceptions/base-exception.hpp"
#include "shader.hpp"
#include <vector>

#include "glad/glad.h"

namespace astralix {

ShaderRenderer::ShaderRenderer() {
  this->id  = glCreateProgram();
  m_uniform = Uniform(id);
}

void ShaderRenderer::use() { glUseProgram(this->id); }

Either<BaseException, Unit> ShaderRenderer::attach_many(ResourceID *shader_ids,
                                                        size_t size) {
  for (int i = 0; i < size; i++) {
    this->attach(shader_ids[i]);
  }

  return Unit();
}

Either<BaseException, Unit> ShaderRenderer::attach(ResourceID p_id) {
  auto resource_manager = Engine::get()->get_resource_manager();
  auto shader_ptr       = resource_manager->get_shader_by_id(p_id);

  ASSERT(shader_ptr == nullptr, "Shader not found");

  glAttachShader(this->id, shader_ptr->vertex);
  glAttachShader(this->id, shader_ptr->fragment);
  if (shader_ptr->geometry)
    glAttachShader(this->id, shader_ptr->geometry);

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
  if (shader_ptr->geometry)
    glDeleteShader(shader_ptr->geometry);

  shader_ptr->signed_to = this->id;

  m_shaders.push_back(p_id);

  return Unit();
}
} // namespace astralix