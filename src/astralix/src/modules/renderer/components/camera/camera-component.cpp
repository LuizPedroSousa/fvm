#include "camera-component.hpp"
#include "entities/object.hpp"

#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"

#include "window.hpp"

#include "application.hpp"
#include "components/resource/resource-component.hpp"
#include "engine.hpp"
#include "events/key-event.hpp"
#include "events/mouse-event.hpp"
#include "iostream"
#include "time.hpp"

namespace astralix {

CameraComponent::CameraComponent() {}

static float s_speed = 2.0f;
static glm::vec3 s_position;
static glm::vec3 s_up = glm::vec3(0.0f, 1.0f, 0.0f);
static glm::vec3 s_front = glm::vec3(0.003012, -0.509046, 0.860734);

static int count = 0;

static glm::vec3 direction;

static float last_x, last_y;

static float offset_x, offset_y;

static float pitch = 0.0f;
static float yaw = -90.0f;

const float sensitivity = 0.1f;

static bool first = true;

CameraComponent::CameraComponent(COMPONENT_INIT_PARAMS, glm::vec3 position)
    : COMPONENT_INIT(CameraComponent), m_is_orthographic(false) {
  s_position = position;
  last_x = Window::get()->get_height() / 2;
  last_y = Window::get()->get_width() / 2;
  m_position = s_position;

  KeyPressedDispatcher *key_pressed_dispatcher = KeyPressedDispatcher::get();
  MouseDispatcher *mouse_dispatcher = MouseDispatcher::get();

  key_pressed_dispatcher->attach(KeyPressedEvent(KeyCode::W), []() {
    count += 1;
    s_position += s_speed * Time::get()->get_deltatime() * direction;
  });

  key_pressed_dispatcher->attach(KeyPressedEvent(KeyCode::A), []() {
    s_position -= s_speed * Time::get()->get_deltatime() *
                  glm::normalize(glm::cross(s_front, s_up));
  });

  key_pressed_dispatcher->attach(KeyPressedEvent(KeyCode::S), []() {
    s_position -= s_speed * Time::get()->get_deltatime() * direction;
  });

  key_pressed_dispatcher->attach(KeyPressedEvent(KeyCode::D), []() {
    s_position += s_speed * Time::get()->get_deltatime() *
                  glm::normalize(glm::cross(s_front, s_up));
  });

  key_pressed_dispatcher->attach(KeyPressedEvent(KeyCode::D), []() {
    s_position += s_speed * Time::get()->get_deltatime() *
                  glm::normalize(glm::cross(s_front, s_up));
  });

  key_pressed_dispatcher->attach(KeyPressedEvent(KeyCode::Space), []() {
    s_position.y += Time::get()->get_deltatime() * 2.0f;
  });

  key_pressed_dispatcher->attach(KeyPressedEvent(KeyCode::LeftControl), []() {
    s_position.y -= Time::get()->get_deltatime() * 2.0f;
  });

  mouse_dispatcher->attach([](MouseEvent event) {
    if (first) {
      last_x = event.get_x();
      last_y = event.get_y();
      first = false;
    }

    offset_x = event.get_x() - last_x;
    offset_y = event.get_y() - last_y;
    last_x = event.get_x();
    last_y = event.get_y();

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

    s_front = glm::normalize(direction);
  });
}

void CameraComponent::use_orthographic() { m_is_orthographic = true; }

void CameraComponent::use_perspective() { m_is_orthographic = false; }

glm::mat4 CameraComponent::get_projection() {
  if (m_is_orthographic) {
    return glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
  } else {
    auto window = Window::get();
    return glm::perspective(
        45.0f, (float)window->get_width() / (float)window->get_height(), 0.1f,
        100.0f);
  }
}

glm::mat4 CameraComponent::get_view() {
  // set static positions
  m_position = s_position;
  m_front = s_front;
  m_up = s_up;

  // view space
  auto transform = glm::mat4(1.0f);
  transform = glm::lookAt(s_position, s_position + s_front, s_up);

  return transform;
}

void CameraComponent::update(Uniform *uniform) {
  uniform->setMatrix("view", get_view());
  uniform->setMatrix("projection", get_projection());
}

} // namespace astralix
