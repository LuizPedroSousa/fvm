#include "resource-component.hpp"

namespace astralix {

ResourceComponent::ResourceComponent(COMPONENT_INIT_PARAMS)
    : COMPONENT_INIT(ResourceComponent, "Resource Renderer", false){};

void ResourceComponent::start() {
  m_texture_renderer.start();
  m_shader_renderer.use();
}

void ResourceComponent::update() {
  m_shader_renderer.use();
  m_texture_renderer.render(m_shader_renderer.get_uniform());
}

ResourceComponent *ResourceComponent::attach_shader(ResourceID id) {
  m_shader_renderer.attach(id);

  return this;
}

ResourceComponent *ResourceComponent::attach_texture(TextureRenderData data) {
  m_texture_renderer.attach(data);
  return this;
}

ResourceComponent *ResourceComponent::attach_cubemap(TextureRenderData data) {
  m_texture_renderer.attach_cubemap(data);
  return this;
}

} // namespace astralix