#pragma once
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "glad/glad.h"

class Shader
{
private:
  Shader(unsigned int vertex, unsigned int fragment);
  static Either<BaseException, GLuint> compile(const char *filename, int type);
  static const char *getFile(const char *filename);

public:
  Shader();

  unsigned int signed_to = 0;
  unsigned int vertex = 0;
  unsigned int fragment = 0;
  Either<BaseException, Shader> static create(const char *vertex_filename, const char *fragment_filename);
  Either<BaseException, Shader *> static createMany(std::pair<const char *, const char *> files[], size_t size);
};
