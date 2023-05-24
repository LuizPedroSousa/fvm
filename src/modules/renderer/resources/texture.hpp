#pragma once
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "filesystem"
#include "shader.hpp"
#include "tuple"
#include "uniform.hpp"

class Texture {
  public:
  unsigned int id;
  const char *name;
  int width;
  int height;
  Texture();
  static Either<BaseException, Texture> create(const char *name, const char *filename, int color_type);
  static Either<BaseException, Texture *> create_many(size_t size, std::initializer_list<std::tuple<const char *, const char *, int>> texturesData);
  Uniform uniform;

  private:
  Texture(unsigned int id, const char *name, int width, int height);
  static std::filesystem::path getPath(const char *filename);
};