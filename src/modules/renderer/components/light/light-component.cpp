#include "light-component.hpp"
#include "components/light/serializers/light-component-serializer.hpp"
#include "components/light/strategies/point-strategy.hpp"
#include "components/resource/resource-component.hpp"
#include "entities/ientity.hpp"
#include "memory"
#include <cmath>

namespace astralix {

LightComponent::LightComponent(COMPONENT_INIT_PARAMS, EntityID camera)
    : COMPONENT_INIT(LightComponent, "light", true,
                     create_ref<LightComponentSerializer>(this)),
      m_camera(camera) {};

void LightComponent::start() {}

void LightComponent::update(Object *object, size_t &index) {
  auto resource = object->get_component<ResourceComponent>();

  if (resource != nullptr && resource->has_shader()) {
    auto owner = get_owner();

    m_strategy->update(owner, object, m_camera, index);
  }
}

} // namespace astralix
