#include "spotlight-strategy.hpp"
#include "components/resource/resource-component.hpp"
#include "components/transform/transform-component.hpp"

namespace astralix {

SpotLightStrategy::SpotLightStrategy() : LightStrategy(){};

void SpotLightStrategy::update(LightSource *source, Object *object,
                               CameraComponent *camera) {
  auto resource = object->get_component<ResourceComponent>();

  resource->get_shader_renderer_uniform()->setVec3("spot_light.position",
                                                   camera->get_position());
  resource->get_shader_renderer_uniform()->setVec3("spot_light.direction",
                                                   camera->get_front());
  resource->get_shader_renderer_uniform()->setFloat(
      "spot_light.inner_cut_off", glm::cos(glm::radians(12.5f)));
  resource->get_shader_renderer_uniform()->setFloat(
      "spot_light.outer_cut_off", glm::cos(glm::radians(20.5f)));
  resource->get_shader_renderer_uniform()->setVec3(
      "spot_light.exposure.ambient", glm::vec3(0.2f));
  resource->get_shader_renderer_uniform()->setVec3(
      "spot_light.exposure.diffuse", glm::vec3(0.5f));
  resource->get_shader_renderer_uniform()->setVec3(
      "spot_light.exposure.specular", glm::vec3(1.0f));
}

} // namespace astralix