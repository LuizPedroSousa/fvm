#include "directional-strategy.hpp"
#include "components/light/light-component.hpp"
#include "components/resource/resource-component.hpp"
#include "components/transform/transform-component.hpp"
#include "events/key-codes.hpp"
#include "events/key-event.hpp"
#include "time.hpp"
#include <imgui.h>

namespace astralix {

void DirectionalStrategy::update(Object *source, Object *object,
                                 Object *camera) {

  auto resource = object->get_component<ResourceComponent>();
  auto transform = source->get_component<TransformComponent>();

  auto shader = resource->get_shader();

  auto entity_manager = EntityManager::get();

  auto light_entity =
      entity_manager->get_entity_with_component<LightComponent>();

  auto trans = light_entity->get_component<TransformComponent>();

  float near_plane = -10.0f, far_plane = 7.5f;

  glm::mat4 lightProjection, lightView;
  glm::mat4 lightSpaceMatrix;

  auto up = glm::vec3(0.0, 1.0, 0.0);

  auto front = glm::vec3(0.0f);

  auto distance = 6.0f;

  lightProjection = glm::ortho(-distance, distance, -distance, distance,
                               near_plane, far_plane);

  lightView = glm::lookAt(trans->position, front, up);

  lightSpaceMatrix = lightProjection * lightView;

  auto event_dispatcher = EventDispatcher::get();

  shader->set_int("light_type", 0);
  shader->set_vec3("directional_light.direction", transform->forward());
  shader->set_vec3("directional_light.position", transform->position);
  shader->set_vec3("directional_light.exposure.ambient", glm::vec3(0.2f));
  shader->set_vec3("directional_light.exposure.diffuse", glm::vec3(0.5f));
  shader->set_vec3("directional_light.exposure.specular", glm::vec3(0.5f));

  shader->set_matrix("light_space_matrix", lightSpaceMatrix);
} // namespace astralix
} // namespace astralix
