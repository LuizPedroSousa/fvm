#include "camera.hpp"
#include "base.hpp"
#include "components/camera/camera-component.hpp"
#include "components/transform/transform-component.hpp"
#include "ecs/entities/ientity.hpp"
#include "events/key-codes.hpp"
#include "events/keyboard.hpp"
#include "glm/geometric.hpp"
#include "log.hpp"
#include "time.hpp"

namespace astralix {

EditorCamera::EditorCamera(ENTITY_INIT_PARAMS, glm::vec3 position)
    : Object(ENTITY_PARAM_NAMES) {
  add_component<TransformComponent>(position);
  add_component<CameraComponent>();
}

void EditorCamera::update() {
  CHECK_ACTIVE(this);

  auto entity_manager = EntityManager::get();
  auto component_manager = ComponentManager::get();

  auto transform = get_component<TransformComponent>();

  auto camera = component_manager->get_component<CameraComponent>();

  if (transform == nullptr || !transform->is_active()) {
    return;
  }

  // if (IS_KEY_DOWN(KeyCode::W)) {
  //   transform->position +=
  //       camera->direction * camera->speed * Time::get()->get_deltatime();
  // }
  //
  // if (IS_KEY_DOWN(KeyCode::S)) {
  //   transform->position -=
  //       camera->direction * camera->speed * Time::get()->get_deltatime();
  // }
  //
  // if (IS_KEY_DOWN(KeyCode::A)) {
  //   transform->position -=
  //       glm::normalize(glm::cross(camera->front, camera->up)) * camera->speed
  //       * Time::get()->get_deltatime();
  // }
  //
  // if (IS_KEY_DOWN(KeyCode::D)) {
  //   transform->position +=
  //       glm::normalize(glm::cross(camera->front, camera->up)) * camera->speed
  //       * Time::get()->get_deltatime();
  // }
  //
  // if (IS_KEY_DOWN(KeyCode::Space)) {
  //   transform->position.y += camera->speed * Time::get()->get_deltatime();
  // }
  //
  // if (IS_KEY_DOWN(KeyCode::LeftControl)) {
  //   transform->position.y -= camera->speed * Time::get()->get_deltatime();
  // }

  transform->update();
}

} // namespace astralix
