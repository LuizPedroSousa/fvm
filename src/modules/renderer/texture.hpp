#pragma once
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "filesystem"

class Texture
{
private:
  Texture(unsigned int id, int width, int height);
  static std::filesystem::path getPath(const char *filename);

public:
  unsigned int id;
  int width;
  int height;
  Texture();
  static Either<BaseException, Texture> create(const char *filename, int color_type);
  static Either<BaseException, Texture *> createMany(size_t size, std::initializer_list<std::pair<const char *, int>> texturesData);
};