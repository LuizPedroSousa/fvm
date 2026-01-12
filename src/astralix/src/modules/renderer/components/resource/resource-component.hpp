#pragma once
#include "components/component.hpp"
#include "resources/shader.hpp"

namespace astralix {

struct TextureRenderData {
  ResourceID id;
  std::string name;
};

class ResourceComponent : public Component<ResourceComponent> {
public:
  ResourceComponent(COMPONENT_INIT_PARAMS);
  ResourceComponent() {}

  void start();
  void update();

  ResourceComponent *attach_texture(TextureRenderData data);
  ResourceComponent *attach_cubemap(TextureRenderData data);
  ResourceComponent *attach_shader(ResourceID id);
  ResourceComponent *set_shader(ResourceID id);

  bool has_shader() { return m_shader != nullptr; };

  Ref<Shader> &get_shader() { return m_shader; }

private:
  Ref<Shader> m_shader = nullptr;
  std::vector<TextureRenderData> m_textures;
  std::vector<TextureRenderData> m_cubemaps;
};

} // namespace astralix
