#include "material-component.hpp"
#include "components/resource/resource-component.hpp"

namespace astralix {

MaterialComponent::MaterialComponent(COMPONENT_INIT_PARAMS)
    : COMPONENT_INIT(MaterialComponent, "Material", true){};

void MaterialComponent::reset_material() {}

void MaterialComponent::update() {
  auto owner = get_owner();

  owner->get_component<ResourceComponent>()->get_shader()->set_float(
      "material.shininess", 32.0f);
}

void MaterialComponent::attach_material(ResourceID material_id) {
  auto owner = get_owner();
  auto resource = owner->get_component<ResourceComponent>();

  auto resource_manager = ResourceManager::get();

  auto material = resource_manager->get_material_by_id(material_id);

  for (int i = 0; i < material->diffuses.size(); i++) {
    resource->attach_texture({material->diffuses[i], get_name("diffuse", i)});
  }

  for (int i = 0; i < material->speculars.size(); i++) {
    resource->attach_texture({material->speculars[i], get_name("specular", i)});
  }
}

std::string MaterialComponent::get_name(const char *prefix, int count) {
  std::string result =
      std::string("materials" + std::string("[") + std::to_string(count) +
                  std::string("]") + prefix);

  return result;
}

void MaterialComponent::attach_materials(std::vector<ResourceID> material_ids) {
  for (int i = 0; i < material_ids.size(); i++) {
    attach_material(material_ids[i]);
  }
}

} // namespace astralix