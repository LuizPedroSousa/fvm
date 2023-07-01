#pragma once
#include "ecs/guid.hpp"
#include "texture.hpp"
#include "uniform.hpp"
#include "vector"

namespace astralix {

struct TextureRenderData {
  ResourceID id;
  std::string name;
};

class TextureRenderer {
public:
  TextureRenderer(){};
  void render(Uniform *uniform);

  void attach(TextureRenderData data);
  void attach_cubemap(TextureRenderData data);
  void attach_many(size_t size, TextureRenderData *data);

  void start();

private:
  std::vector<TextureRenderData> m_textures;
  std::vector<TextureRenderData> m_cubemaps;
};

} // namespace astralix