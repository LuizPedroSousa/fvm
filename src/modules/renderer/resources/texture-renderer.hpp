#pragma once
#include "./texture.hpp"
#include "uniform.hpp"
#include "utils/guid.hpp"
#include "vector"

class TextureRenderer {
  public:
  TextureRenderer(){};
  void render(Uniform *uniform);

  void attach(ResourceID texture);
  void attach_many(size_t size, ResourceID *textures);

  void start();

  private:
  std::vector<ResourceID> m_textures;
};