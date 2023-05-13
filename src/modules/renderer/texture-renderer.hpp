#pragma once
#include "./texture.hpp"
#include "vector"

class TextureRenderer
{
public:
  TextureRenderer(){};
  std::vector<Texture> textures;
  void render();
  void attach(Texture texture);
  void attachMany(size_t size, Texture *textures);
  Either<BaseException, Unit> attachMany(size_t size, Either<BaseException, Texture *> textures);

  void start();
};