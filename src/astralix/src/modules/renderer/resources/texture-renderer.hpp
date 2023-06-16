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

  void attach(ResourceID texture_id);
  void attach_cubemap(ResourceID cubemap_ids);
  void attach_many(size_t size, ResourceID *texture_ids);

  void start();

private:
  std::vector<ResourceID> m_textures;
  std::vector<ResourceID> m_cubemaps;
};

} // namespace astralix