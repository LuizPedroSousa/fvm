#include "light-component.hpp"
#include "application.hpp"
#include "components/light/serializers/light-component-serializer.hpp"
#include "components/resource/resource-component.hpp"
#include "components/transform/transform-component.hpp"
#include "engine.hpp"
#include "iostream"
#include "log.hpp"
#include "memory"
#include "resources/shader.hpp"

namespace astralix {

LightComponent::LightComponent(COMPONENT_INIT_PARAMS,
                               Scope<LightStrategy> strategy, EntityID camera)
    : COMPONENT_INIT(LightComponent, "Light", true,
                     create_ref<LightComponentSerializer>(this)),
      m_camera(camera), m_strategy(std::move(strategy)) {};

void LightComponent::start() {}

void LightComponent::update(Object *object) {
  auto resource = object->get_component<ResourceComponent>();

  auto camera_entity = EntityManager::get()->get_entity<Object>(m_camera);

  if (resource != nullptr && resource->has_shader()) {
    m_strategy->update(get_owner<Object>(), object, camera_entity);
  }
}

} // namespace astralix
