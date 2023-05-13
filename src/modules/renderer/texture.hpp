#pragma once
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "filesystem"
#include "tuple"
#include "shader.hpp"

class Texture
{
private:
  Texture(unsigned int id, const char *name, int width, int height, Shader shader);
  static std::filesystem::path getPath(const char *filename);

public:
  unsigned int id;
  const char *name;
  Shader shader;
  int width;
  int height;
  Texture();
  static Either<BaseException, Texture> create(const char *name, const char *filename, int color_type, Shader shader);
  static Either<BaseException, Texture *> createMany(size_t size, std::initializer_list<std::tuple<const char *, const char *, int, Shader>> texturesData);
};