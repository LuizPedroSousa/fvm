#include "light-component.hpp"
#include "application.hpp"
#include "components/light/serializers/light-component-serializer.hpp"
#include "components/resource/resource-component.hpp"
#include "components/transform/transform-component.hpp"
#include "ecs/entities/ientity.hpp"
#include "engine.hpp"
#include "iostream"
#include "log.hpp"
#include "memory"
#include "resources/shader.hpp"
#include <cmath>

namespace astralix {

LightComponent::LightComponent(COMPONENT_INIT_PARAMS,
                               Scope<LightStrategy> strategy, EntityID camera)
    : COMPONENT_INIT(LightComponent, "Light", true,
                     create_ref<LightComponentSerializer>(this)),
      m_camera(camera), m_strategy(std::move(strategy)) {};

void LightComponent::start() {}

void LightComponent::update(IEntity *source, Object *object) {
  auto resource = object->get_component<ResourceComponent>();

  if (resource != nullptr && resource->has_shader()) {
    m_strategy->update(source, object, m_camera);
  }
}

} // namespace astralix
