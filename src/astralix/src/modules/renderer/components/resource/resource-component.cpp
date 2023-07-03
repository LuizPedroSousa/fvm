#include "resource-component.hpp"

namespace astralix {

ResourceComponent::ResourceComponent(COMPONENT_INIT_PARAMS)
    : COMPONENT_INIT(ResourceComponent, "Resource Renderer", false){};

void ResourceComponent::start() {
  if (has_shader()) {
    m_shader->bind();
    m_texture_renderer.start();
  }
}

void ResourceComponent::update() {
  if (has_shader()) {
    m_shader->bind();
    m_texture_renderer.render(m_shader);
  }
}

ResourceComponent *ResourceComponent::set_shader(ResourceID id) {
  auto resource_manager = ResourceManager::get();
  auto shader_ptr = resource_manager->get_shader_by_id(id);

  ASTRA_ASSERT_THROW(shader_ptr == nullptr, "Shader not found");

  shader_ptr->attach();

  m_shader = shader_ptr;

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