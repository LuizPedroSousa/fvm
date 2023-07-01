#include "directional-strategy.hpp"
#include "components/resource/resource-component.hpp"
#include "components/transform/transform-component.hpp"

namespace astralix {

void DirectionalStrategy::update(Object *source, Object *object,
                                 Object *camera) {

  auto resource = object->get_component<ResourceComponent>();
  auto transform = source->get_component<TransformComponent>();

  resource->get_shader_renderer_uniform()->setVec3(
      "directional_light.direction", transform->position);
  resource->get_shader_renderer_uniform()->setVec3(
      "directional_light.exposure.ambient", glm::vec3(0.2f));
  resource->get_shader_renderer_uniform()->setVec3(
      "directional_light.exposure.diffuse", glm::vec3(0.5f));
  resource->get_shader_renderer_uniform()->setVec3(
      "directional_light.exposure.specular", glm::vec3(1.0f));
}
} // namespace astralix