#include "point-strategy.hpp"
#include "components/resource/resource-component.hpp"
#include "components/transform/transform-component.hpp"

namespace astralix {

void PointStrategy::update(Object *source, Object *object, Object *camera) {
  auto resource = object->get_component<ResourceComponent>();
  auto transform = source->get_component<TransformComponent>();

  auto shader = resource->get_shader();

  shader->set_float("point_lights[0].attenuation.constant", 1.0f);
  shader->set_float("point_lights[0].attenuation.linear", 0.045f);
  shader->set_float("point_lights[0].attenuation.quadratic", 0.0075f);
  shader->set_vec3("point_lights[0].exposure.ambient", glm::vec3(5.0f));
  shader->set_vec3("point_lights[0].exposure.diffuse", glm::vec3(2.0f));
  shader->set_vec3("point_lights[0].exposure.specular", glm::vec3(1.0f));
  shader->set_vec3("point_lights[0].position", transform->position);
}

} // namespace astralix
