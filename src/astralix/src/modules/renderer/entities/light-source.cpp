#include "light-source.hpp"

#include "components/material/material-component.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/resource/resource-component.hpp"
#include "components/transform/transform-component.hpp"

namespace astralix {

LightSource::LightSource(ENTITY_INIT_PARAMS, glm::vec3 position,
                         glm::vec3 scale)
    : Entity(id, component_manager) {
  add_component<TransformComponent>(position, scale);
  add_component<ResourceComponent>();
}

void LightSource::start() {
  get_component<TransformComponent>()->start();
  get_component<ResourceComponent>()->start();
}

void LightSource::update() {
  get_component<TransformComponent>()->update();
  get_component<ResourceComponent>()->update();
}

} // namespace astralix
