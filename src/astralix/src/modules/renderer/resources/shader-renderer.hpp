#pragma once

#include "ecs/guid.hpp"
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "shader.hpp"
#include "uniform.hpp"
#include <vector>

namespace astralix {

class ShaderRenderer {
public:
  ShaderRenderer();
  unsigned int id;
  void use();

  Uniform *get_uniform() { return &m_uniform; }

  void attach_many(ResourceID *shaders, size_t size);
  void attach(ResourceID shader);

private:
  Uniform m_uniform;
  std::vector<ResourceID> m_shaders;
};

} // namespace astralix