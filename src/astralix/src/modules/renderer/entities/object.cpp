#include "object.hpp"
#include "components/material/material-component.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/resource/resource-component.hpp"
#include "components/transform/transform-component.hpp"

namespace astralix {
Object::Object(ENTITY_INIT_PARAMS, glm::vec3 position, glm::vec3 scale)
    : Entity(id, component_manager) {
  add_component<ResourceComponent>();
  add_component<MeshComponent>();
  add_component<TransformComponent>(position, scale);
  add_component<MaterialComponent>();
}

void Object::start() {
  if (is_active()) {
    auto resource  = get_component<ResourceComponent>();
    auto mesh      = get_component<MeshComponent>();
    auto transform = get_component<TransformComponent>();

    mesh->start();
    resource->start();
    transform->start();
  };
}

void Object::update() {
  if (is_active()) {
    auto transform = get_component<TransformComponent>();
    auto material  = get_component<MaterialComponent>();
    auto resource  = get_component<ResourceComponent>();
    auto mesh      = get_component<MeshComponent>();

    resource->update();
    transform->update();
    material->update();
    mesh->update();
  };
}

} // namespace astralix
