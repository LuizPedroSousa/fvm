#include "resource-component.hpp"
#include "components/resource/serializers/resource-component-serializer.hpp"
#include "log.hpp"
#include "managers/resource-manager.hpp"
#include "resources/shader.hpp"

namespace astralix {

ResourceComponent::ResourceComponent(COMPONENT_INIT_PARAMS)
    : COMPONENT_INIT(ResourceComponent, "Resource Renderer", false,
                     create_ref<ResourceComponentSerializer>(this)) {};

void ResourceComponent::start() {
  if (has_shader()) {
    m_shader->bind();
  }
}

void ResourceComponent::update() {
  if (has_shader()) {
    m_shader->bind();

    auto resource_manager = ResourceManager::get();

    for (auto cubemap : m_cubemaps) {
      auto cubemap_ptr = resource_manager->get_texture_by_id(cubemap.id);

      ASTRA_EXCEPTION(cubemap_ptr == nullptr,
                      "ResourceComponent::update() ERROR: Failed to render - "
                      "Texture3D with ID [" +
                          cubemap.id + "] not found in ResourceManager.");

      cubemap_ptr->active(cubemap_ptr->get_slot());

      m_shader->set_int(cubemap.name, cubemap_ptr->get_slot());

      cubemap_ptr->bind();
    }

    for (auto texture : m_textures) {
      auto texture_ptr = resource_manager->get_texture_by_id(texture.id);

      ASTRA_EXCEPTION(texture_ptr == nullptr,
                      "ResourceComponent::update() ERROR: Failed to render - "
                      "Texture2D with ID [" +
                          texture.id + "] not found in ResourceManager.");

      texture_ptr->active(texture_ptr->get_slot());

      m_shader->set_int(texture.name.c_str(), texture_ptr->get_slot());

      texture_ptr->bind();
    }
  }
}

ResourceComponent *ResourceComponent::attach_shader(ResourceID id) {
  auto resource_manager = ResourceManager::get();
  auto shader_ptr = resource_manager->get_shader_by_id(id);

  ASTRA_EXCEPTION(shader_ptr == nullptr,
                  "Failed to set shader: Shader with ID [" + id +
                      "] not found in ResourceManager.");

  shader_ptr->attach();

  m_shader = shader_ptr;

  return this;
}

ResourceComponent *ResourceComponent::set_shader(ResourceID id) {
  auto resource_manager = ResourceManager::get();
  auto shader_ptr = resource_manager->get_shader_by_id(id);

  ASTRA_EXCEPTION(shader_ptr == nullptr,
                  "Failed to set shader: Shader with ID [" + id +
                      "] not found in ResourceManager.");

  m_shader = shader_ptr;

  return this;
}

ResourceComponent *ResourceComponent::attach_texture(TextureRenderData data) {
  LOG_INFO("Attaching texture with ID: " + data.id);

  auto resource_manager = ResourceManager::get();
  auto texture_ptr = resource_manager->get_texture_by_id(data.id);

  ASTRA_EXCEPTION(texture_ptr == nullptr,
                  "Failed to attach texture: Texture with ID [" + data.id +
                      "] not found in ResourceManager.");

  m_textures.push_back(data);

  return this;
}

ResourceComponent *ResourceComponent::attach_cubemap(TextureRenderData data) {
  m_cubemaps.push_back(data);

  return this;
}

} // namespace astralix
