#include "light-component.hpp"
#include "application.hpp"
#include "components/resource/resource-component.hpp"
#include "components/transform/transform-component.hpp"
#include "engine.hpp"
#include "iostream"
#include "memory"
#include "resources/shader.hpp"

namespace astralix {

LightComponent::LightComponent(COMPONENT_INIT_PARAMS,
                               Scope<LightStrategy> strategy, EntityID camera)
    : COMPONENT_INIT(LightComponent, "Light", true), m_camera(camera),
      m_strategy(std::move(strategy)){};

void LightComponent::start() {}

void LightComponent::update(Object *object) {
  auto resource = object->get_component<ResourceComponent>();

  auto camera_entity =
      EntityManager::get()->get_entity<Object>(m_camera);

  auto camera_component = camera_entity->get_component<CameraComponent>();
  auto camera_transform = camera_entity->get_component<TransformComponent>();

  if (resource != nullptr) {
    resource->get_shader_renderer_uniform()->setVec3(
        "view_position", camera_transform->position);
  }

  m_strategy->update(get_owner<Object>(), object, camera_entity);
}

} // namespace astralix
