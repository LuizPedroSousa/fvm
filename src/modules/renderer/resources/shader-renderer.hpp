#pragma once

#include "./shader.hpp"
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "uniform.hpp"
#include <vector>

class ShaderRenderer {
  public:
  ShaderRenderer();
  unsigned int id;
  void use();

  Uniform *get_uniform() {
    return &m_uniform;
  }

  std::vector<Shader> shaders;
  Either<BaseException, Unit> attach_many(Shader *shaders, size_t size);
  Either<BaseException, Unit> attach(Shader shaders);
  Either<BaseException, Unit> attach(Either<BaseException, Shader> shader);

  private:
  Uniform m_uniform;
};