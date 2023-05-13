#pragma once
#include "glad/glad.h"
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include <vector>
#include "./shader.hpp"

class ShaderRenderer
{
public:
  ShaderRenderer();
  unsigned int id;
  void use();
  std::vector<Shader> shaders;

  Either<BaseException, Unit> attachMany(Shader *shaders, size_t size);
  Either<BaseException, Unit> attach(Shader shaders);
};