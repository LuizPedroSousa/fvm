#include "spotlight-strategy.hpp"
#include "components/camera/camera-component.hpp"
#include "components/resource/resource-component.hpp"
#include "components/transform/transform-component.hpp"

namespace astralix {

SpotLightStrategy::SpotLightStrategy() : LightStrategy(){};

void SpotLightStrategy::update(Object *source, Object *object, Object *camera) {
  auto resource = object->get_component<ResourceComponent>();

  auto camera_transform = camera->get_component<TransformComponent>();
  auto camera_component = camera->get_component<CameraComponent>();

  resource->get_shader_renderer_uniform()->setVec3("spot_light.position",
                                                   camera_transform->position);
  resource->get_shader_renderer_uniform()->setVec3("spot_light.direction",
                                                   camera_component->front);
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