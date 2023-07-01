#pragma once
#include "ecs/components/component.hpp"
#include "resources/shader-renderer.hpp"
#include "resources/shader.hpp"
#include "resources/texture-renderer.hpp"

namespace astralix {

class ResourceComponent : public Component<ResourceComponent> {
public:
  ResourceComponent(COMPONENT_INIT_PARAMS);
  ResourceComponent() {}

  void start();
  void update();

  unsigned int get_shader_renderer_id() { return m_shader_renderer.id; }

  ShaderRenderer *get_shader_renderer() { return &m_shader_renderer; }

  Uniform *get_shader_renderer_uniform() {
    return m_shader_renderer.get_uniform();
  }

  ResourceComponent *attach_texture(TextureRenderData data);
  ResourceComponent *attach_cubemap(TextureRenderData data);
  ResourceComponent *attach_shader(ResourceID id);

private:
  TextureRenderer m_texture_renderer;
  ShaderRenderer m_shader_renderer;
};

} // namespace astralix