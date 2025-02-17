#include "object.hpp"
#include "components/light/light-component.hpp"
#include "components/material/material-component.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/resource/resource-component.hpp"
#include "components/transform/transform-component.hpp"
#include "ecs/entities/ientity.hpp"

namespace astralix {
Object::Object(ENTITY_INIT_PARAMS, glm::vec3 position, glm::vec3 scale)
    : ENTITY_INIT() {
  add_component<ResourceComponent>();
  add_component<TransformComponent>(position, scale);
}

Object::Object(ObjectDTO dto) : ENTITY_INIT_DTO(dto) {
  add_component<ResourceComponent>();
  add_component<TransformComponent>(dto.position, dto.scale);
}

void Object::start() {
  if (is_active()) {
    auto resource = get_component<ResourceComponent>();
    auto mesh = get_component<MeshComponent>();
    auto transform = get_component<TransformComponent>();

    if (resource != nullptr && resource->is_active())
      resource->start();

    auto shader = resource->get_shader();

    if (shader != nullptr) {
      shader->bind();
      shader->set_int("shadowMap", 1);
    }

    if (transform != nullptr && transform->is_active())
      transform->start();

    if (mesh != nullptr && mesh->is_active())
      mesh->start();
  };
}

void Object::pre_update() {}

void Object::update() {
  if (is_active()) {
    auto entity_manager = EntityManager::get();
    auto component_manager = ComponentManager::get();

    auto resource = get_component<ResourceComponent>();
    auto mesh = get_component<MeshComponent>();
    auto transform = get_component<TransformComponent>();

    auto material = get_component<MaterialComponent>();

    resource->update();

    if (entity_manager->has_entity_with_component<LightComponent>()) {
      component_manager->get_component<LightComponent>()->update(this);
    }

    if (entity_manager->has_entity_with_component<CameraComponent>() &&
        resource->has_shader()) {
      auto shader = resource->get_shader();
      auto camera = component_manager->get_component<CameraComponent>();

      camera->update(shader);
    }

    if (transform != nullptr && transform->is_active())
      transform->update();

    if (material != nullptr && material->is_active()) {
      material->update();
    }

    if (mesh != nullptr && mesh->is_active()) {
      mesh->update();
    }
  }
}

void Object::post_update() {}

} // namespace astralix
