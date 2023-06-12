#pragma once
#include "ecs/guid.hpp"
#include "texture.hpp"
#include "uniform.hpp"
#include "vector"

namespace astralix {

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

} // namespace astralix