#include "camera-component.hpp"
#include "components/transform/transform-component.hpp"
#include "entities/object.hpp"

#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"

#include "window.hpp"

#include "application.hpp"
#include "components/resource/resource-component.hpp"
#include "engine.hpp"
#include "events/event-dispatcher.hpp"
#include "events/key-event.hpp"
#include "events/mouse-event.hpp"
#include "iostream"
#include "time.hpp"

namespace astralix {

CameraComponent::CameraComponent(COMPONENT_INIT_PARAMS)
    : COMPONENT_INIT(CameraComponent, "Camera", true),
      m_is_orthographic(false) {
  last_offset_x = Window::get()->get_height() / 2;
  last_offset_y = Window::get()->get_width() / 2;

  EventDispatcher *event_dispatcher = EventDispatcher::get();

  event_dispatcher->attach<KeyboardListener, KeyPressedEvent>(
      KeyCode::W, [&]() {
        auto owner = get_owner();

        auto camera = owner->get_component<CameraComponent>();
        auto transform = owner->get_component<TransformComponent>();

        transform->position +=
            camera->speed * Time::get()->get_deltatime() * camera->direction;
      });

  event_dispatcher->attach<KeyboardListener, KeyPressedEvent>(
      KeyCode::A, [&]() {
        IEntity *owner = get_owner();
        auto transform = owner->get_component<TransformComponent>();
        auto camera = owner->get_component<CameraComponent>();

        transform->position -=
            camera->speed * Time::get()->get_deltatime() *
            glm::normalize(glm::cross(camera->front, camera->up));
      });

  event_dispatcher->attach<KeyboardListener, KeyPressedEvent>(
      KeyCode::S, [&]() {
        IEntity *owner = get_owner();
        auto transform = owner->get_component<TransformComponent>();
        auto camera = owner->get_component<CameraComponent>();

        transform->position -=
            camera->speed * Time::get()->get_deltatime() * camera->direction;
      });

  event_dispatcher->attach<KeyboardListener, KeyPressedEvent>(
      KeyCode::D, [&]() {
        IEntity *owner = get_owner();
        auto transform = owner->get_component<TransformComponent>();
        auto camera = owner->get_component<CameraComponent>();

        transform->position +=
            camera->speed * Time::get()->get_deltatime() *
            glm::normalize(glm::cross(camera->front, camera->up));
      });

  event_dispatcher->attach<KeyboardListener, KeyPressedEvent>(

      KeyCode::Space, [&]() {
        auto transform = get_owner()->get_component<TransformComponent>();
        transform->position.y += Time::get()->get_deltatime() * 2.0f;
      });

  event_dispatcher->attach<KeyboardListener, KeyPressedEvent>(
      KeyCode::LeftControl, [&]() {
        auto owner = EntityManager::get()->get_entity<Object>(get_owner_id());

        auto transform = owner->get_component<TransformComponent>();
        transform->position.y -= Time::get()->get_deltatime() * 2.0f;
      });

  event_dispatcher->attach<MouseListener, MouseEvent>([&](MouseEvent *event) {
    IEntity *owner = get_owner();
    auto transform = owner->get_component<TransformComponent>();
    auto camera = owner->get_component<CameraComponent>();

    camera->recalculate_camera_rotation(event->get_x(), event->get_y());
  });
}

void CameraComponent::use_orthographic() { m_is_orthographic = true; }

void CameraComponent::use_perspective() { m_is_orthographic = false; }

void CameraComponent::recalculate_projection_matrix() {
  if (m_is_orthographic) {
    m_projection_matrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
  } else {

    auto window = Window::get();
    const auto &spec = Engine::get()->framebuffer->get_specification();

    m_projection_matrix = glm::perspective(
        45.0f, (float)spec.width / (float)spec.height, 0.1f, 100.0f);
  }
}

void CameraComponent::recalculate_view_matrix() {
  // view space
  auto matrix = glm::mat4(1.0f);

  auto transform = get_owner()->get_component<TransformComponent>();

  matrix = glm::lookAt(transform->position, transform->position + front, up);

  m_view_matrix = matrix;
}

void CameraComponent::recalculate_camera_rotation(float x, float y) {
  if (is_first_recalculation) {
    last_offset_x = x;
    last_offset_y = y;
    is_first_recalculation = false;
  }

  offset_x = x - last_offset_x;
  offset_y = y - last_offset_y;
  last_offset_x = x;
  last_offset_y = y;

  offset_x *= sensitivity;
  offset_y *= sensitivity;

  yaw += offset_x;
  pitch -= offset_y;

  // limit rotation of y axis
  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  front = glm::normalize(direction);
}

void CameraComponent::update(Uniform *uniform) {
  recalculate_view_matrix();
  recalculate_projection_matrix();

  uniform->setMatrix("view", m_view_matrix);
  uniform->setMatrix("projection", m_projection_matrix);
}

} // namespace astralix
