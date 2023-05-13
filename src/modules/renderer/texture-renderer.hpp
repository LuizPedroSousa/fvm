#pragma once
#include "./texture.hpp"
#include "vector"

class TextureRenderer
{
public:
  TextureRenderer(){};
  std::vector<Texture> textures;
  void use();
  void attach(Texture texture);
  void attachMany(size_t size, Texture *textures);
};