#include "point-strategy.hpp"
#include "components/resource/resource-component.hpp"
#include "components/transform/transform-component.hpp"

namespace astralix {

void PointStrategy::update(LightSource *source, Object *object,
                           CameraComponent *camera) {
  auto resource = object->get_component<ResourceComponent>();
  auto transform = source->get_component<TransformComponent>();

  resource->get_shader_renderer_uniform()->setFloat(
      "point_lights[0].attenuation.constant", 1.0f);
  resource->get_shader_renderer_uniform()->setFloat(
      "point_lights[0].attenuation.linear", 0.09f);
  resource->get_shader_renderer_uniform()->setFloat(
      "point_lights[0].attenuation.quadratic", 0.032f);
  resource->get_shader_renderer_uniform()->setVec3(
      "point_lights[0].exposure.ambient", glm::vec3(1.0f));
  resource->get_shader_renderer_uniform()->setVec3(
      "point_lights[0].exposure.diffuse", glm::vec3(0.5f));
  resource->get_shader_renderer_uniform()->setVec3(
      "point_lights[0].exposure.specular", glm::vec3(1.0f));
  resource->get_shader_renderer_uniform()->setVec3(
      "point_lights[0].position", transform->get_render_transform()->position);
}

} // namespace astralix
