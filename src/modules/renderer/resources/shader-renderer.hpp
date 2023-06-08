#pragma once

#include "./shader.hpp"
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "uniform.hpp"
#include "utils/guid.hpp"
#include <vector>

class ShaderRenderer {
  public:
  ShaderRenderer();
  unsigned int id;
  void use();

  Uniform *get_uniform() {
    return &m_uniform;
  }

  Either<BaseException, Unit> attach_many(ResourceID *shaders, size_t size);
  Either<BaseException, Unit> attach(ResourceID shader);

  private:
  Uniform m_uniform;
  std::vector<ResourceID> m_shaders;
};