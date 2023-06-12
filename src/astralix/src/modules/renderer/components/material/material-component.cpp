#include "material-component.hpp"
#include "components/resource/resource-component.hpp"
#include "engine.hpp"
#include "glad/glad.h"

namespace astralix {

MaterialComponent::MaterialComponent(COMPONENT_INIT_PARAMS)
    : COMPONENT_INIT(MaterialComponent){};

void MaterialComponent::reset_material() {}

void MaterialComponent::update() {
  auto owner = Engine::get()->get_owner(get_owner_id());

  owner->get_component<ResourceComponent>()
      ->get_shader_renderer_uniform()
      ->setFloat("material.shininess", 32.0f);
}

void MaterialComponent::attach_material(ResourceID material_id) {
  auto owner = Engine::get()->get_owner(get_owner_id());
  auto resource = owner->get_component<ResourceComponent>();

  auto resource_manager = Engine::get()->get_resource_manager();

  auto material = resource_manager->get_material_by_id(material_id);

  for (int i = 0; i < material->diffuses.size(); i++) {
    resource->attach_texture(material->diffuses[i]);
  }

  for (int i = 0; i < material->speculars.size(); i++) {
    resource->attach_texture(material->speculars[i]);
  }
}

void MaterialComponent::attach_materials(std::vector<ResourceID> material_ids) {
  for (int i = 0; i < material_ids.size(); i++) {
    attach_material(material_ids[i]);
  }
}

} // namespace astralix