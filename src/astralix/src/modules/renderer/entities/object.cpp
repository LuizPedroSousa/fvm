#include "object.hpp"
#include "components/light/light-component.hpp"
#include "components/material/material-component.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/resource/resource-component.hpp"
#include "components/skybox/skybox-component.hpp"
#include "components/transform/transform-component.hpp"
#include "engine.hpp"

namespace astralix {
Object::Object(ENTITY_INIT_PARAMS, glm::vec3 position, glm::vec3 scale)
    : ENTITY_INIT() {
  add_component<ResourceComponent>();
  add_component<TransformComponent>(position, scale);
}

void Object::start() {
  if (is_active()) {
    auto resource = get_component<ResourceComponent>();
    auto mesh = get_component<MeshComponent>();
    auto transform = get_component<TransformComponent>();
    auto skybox = get_component<SkyboxComponent>();

    if (resource != nullptr && resource->is_active())
      resource->start();

    if (skybox != nullptr && skybox->is_active()) {
      skybox->start();
    }

    if (transform != nullptr && transform->is_active())
      transform->start();

    if (mesh != nullptr && mesh->is_active())
      mesh->start();
  };
}

void Object::pre_update() {
  if (is_active()) {

    auto skybox = get_component<SkyboxComponent>();

    if (skybox != nullptr && skybox->is_active()) {
      skybox->pre_update();
    }
  }
}

void Object::update() {
  if (is_active()) {
    auto entity_manager = EntityManager::get();
    auto component_manager = ComponentManager::get();

    auto resource = get_component<ResourceComponent>();
    auto mesh = get_component<MeshComponent>();
    auto transform = get_component<TransformComponent>();

    auto skybox = get_component<SkyboxComponent>();
    auto material = get_component<MaterialComponent>();

    resource->update();

    if (entity_manager->has_entity_with_component<LightComponent>()) {
      component_manager->get_component<LightComponent>()->update(this);
    }

    if (entity_manager->has_entity_with_component<CameraComponent>()) {
      component_manager->get_component<CameraComponent>()->update(
          resource->get_shader_renderer_uniform());
    }

    if (skybox != nullptr && skybox->is_active()) {
      skybox->update();
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

void Object::post_update() {
  if (is_active()) {
    auto skybox = get_component<SkyboxComponent>();

    if (skybox != nullptr) {
      skybox->post_update();
    }
  }
}

} // namespace astralix
