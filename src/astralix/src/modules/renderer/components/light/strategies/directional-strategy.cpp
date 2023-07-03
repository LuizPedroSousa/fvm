#include "directional-strategy.hpp"
#include "components/resource/resource-component.hpp"
#include "components/transform/transform-component.hpp"

namespace astralix {

void DirectionalStrategy::update(Object *source, Object *object,
                                 Object *camera) {

  auto resource = object->get_component<ResourceComponent>();
  auto transform = source->get_component<TransformComponent>();

  auto shader = resource->get_shader();

  shader->set_vec3("directional_light.direction", transform->position);
  shader->set_vec3("directional_light.exposure.ambient", glm::vec3(0.2f));
  shader->set_vec3("directional_light.exposure.diffuse", glm::vec3(0.5f));
  shader->set_vec3("directional_light.exposure.specular", glm::vec3(1.0f));
}
} // namespace astralix