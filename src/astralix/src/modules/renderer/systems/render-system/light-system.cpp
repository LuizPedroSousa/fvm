#include "light-system.hpp"

#include "base.hpp"
#include "components/camera/camera-component.hpp"
#include "components/light/light-component.hpp"
#include "components/resource/resource-component.hpp"
#include "components/transform/transform-component.hpp"
#include "ecs/managers/entity-manager.hpp"
#include "entities/camera.hpp"
#include "entities/object.hpp"
#include "events/event.hpp"
#include "glad//glad.h"
#include "managers/resource-manager.hpp"
#include "systems/render-system/shadow-mapping-system.hpp"
#include "trace.hpp"
#include <cmath>

namespace astralix {

LightSystem::LightSystem() {}

void LightSystem::pre_update(double dt) {}

void LightSystem::fixed_update(double fixed_dt) {}

void LightSystem::start() { auto resource_manager = ResourceManager::get(); }

void LightSystem::update(double dt) {
  CHECK_ACTIVE(this);
  ASTRA_PROFILE_N("LightSystem Update");

  auto entity_manager = EntityManager::get();

  auto objects = entity_manager->get_entities<Object>();

  entity_manager->for_each<Camera>([&](Camera *object) {
    object->update();

    auto camera = object->get_component<CameraComponent>();

    camera->recalculate_projection_matrix();
    camera->recalculate_view_matrix();
  });

  auto system_manager = SystemManager::get();

  auto shadow_mapping = system_manager->get_system<ShadowMappingSystem>();

  LightComponent *light_component;

  auto light = entity_manager->get_entity_with_component<LightComponent>();

  if (light != nullptr) {
    light_component = light->get_component<LightComponent>();
  }

  auto camera = entity_manager->get_entity_with_component<CameraComponent>()
                    ->get_component<CameraComponent>();

  entity_manager->for_each<Object>([&](Object *object) {
    ASTRA_PROFILE_N("LightSystem Object Loop");

    if (shadow_mapping != nullptr) {
      shadow_mapping->bind_depth(object);
    }

    object->update();

    if (light_component != nullptr) {
      light_component->update(light, object);
    }

    auto resource = object->get_component<ResourceComponent>();

    if (!resource->has_shader()) {
      return;
    }

    auto shader = resource->get_shader();

    auto transform = object->get_component<TransformComponent>();

    shader->set_matrix("view", camera->get_view_matrix());
    shader->set_vec3("view_position", transform->position);
    shader->set_matrix("projection", camera->get_projection_matrix());
  });
}

LightSystem::~LightSystem() {}

} // namespace astralix
