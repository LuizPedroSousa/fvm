#include "resource-component.hpp"

namespace astralix {

ResourceComponent::ResourceComponent(COMPONENT_INIT_PARAMS)
    : COMPONENT_INIT(ResourceComponent){};

void ResourceComponent::start() {
  m_texture_renderer.start();
  m_shader_renderer.use();
}

void ResourceComponent::pre_update() {
  m_shader_renderer.use();
  m_texture_renderer.render(m_shader_renderer.get_uniform());
}

void ResourceComponent::attach_shader(ResourceID id) {
  m_shader_renderer.attach(id);
}

void ResourceComponent::attach_texture(ResourceID id) {
  m_texture_renderer.attach(id);
}

} // namespace astralix