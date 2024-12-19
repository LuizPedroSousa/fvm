#pragma once
#include "ecs/components/component.hpp"
#include "resources/shader.hpp"
#include "resources/texture-renderer.hpp"

namespace astralix {

  class ResourceComponent : public Component<ResourceComponent>
  {
  public:
    ResourceComponent(COMPONENT_INIT_PARAMS);
    ResourceComponent() {}

    void start();
    void update();

    ResourceComponent* attach_texture(TextureRenderData data);
    ResourceComponent* attach_cubemap(TextureRenderData data);
    ResourceComponent* set_shader(ResourceID id);

    bool has_shader() { return m_shader != nullptr; };

    Ref<Shader>& get_shader() { return m_shader; }

  private:
    TextureRenderer m_texture_renderer;
    Ref<Shader> m_shader;
  };

} // namespace astralix