#include "./camera.hpp"

#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"

#include "window.hpp"

#include "application.hpp"
#include "events/key-event.hpp"
#include "events/mouse-event.hpp"
#include "iostream"

Camera::Camera() {
}

static float s_speed = 2.0f;
static glm::vec3 s_position;
static glm::vec3 s_up = glm::vec3(0.0f, 1.0f, 0.0f);
static glm::vec3 s_front = glm::vec3(0.0f, 0.0f, -1.0f);

static int count = 0;

static glm::vec3 direction;

static float last_x = Window::get()->get_height() / 2, last_y = Window::get()->get_width() / 2;

static float offset_x, offset_y;

static float pitch = 0.0f;
static float yaw = -90.0f;

const float sensitivity = 0.1f;

static bool first = true;

Camera::Camera(unsigned int render_id, glm::vec3 position) {
  m_uniform = Uniform(render_id);

  m_transform = glm::mat4(1.0f);

  s_position = position;
  m_position = s_position;

  KeyPressedDispatcher *key_pressed_dispatcher = KeyPressedDispatcher::get();
  MouseDispatcher *mouse_dispatcher = MouseDispatcher::get();

  key_pressed_dispatcher->attach(KeyPressedEvent(KeyCode::W), []() {
    count += 1;
    s_position += s_speed * Application::get_deltatime() * direction;
  });

  key_pressed_dispatcher->attach(KeyPressedEvent(KeyCode::A), []() {
    s_position -= s_speed * Application::get_deltatime() * glm::normalize(glm::cross(s_front, s_up));
  });

  key_pressed_dispatcher->attach(KeyPressedEvent(KeyCode::S), []() {
    s_position -= s_speed * Application::get_deltatime() * direction;
  });

  key_pressed_dispatcher->attach(KeyPressedEvent(KeyCode::D), []() {
    s_position += s_speed * Application::get_deltatime() * glm::normalize(glm::cross(s_front, s_up));
  });

  key_pressed_dispatcher->attach(KeyPressedEvent(KeyCode::D), []() {
    s_position += s_speed * Application::get_deltatime() * glm::normalize(glm::cross(s_front, s_up));
  });

  key_pressed_dispatcher->attach(KeyPressedEvent(KeyCode::Space), []() {
    s_position.y += Application::get_deltatime() * 2.0f;
  });

  key_pressed_dispatcher->attach(KeyPressedEvent(KeyCode::LeftControl), []() {
    s_position.y -= Application::get_deltatime() * 2.0f;
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

void Camera::render() {
  // set static positions
  m_position = s_position;
  m_front = s_front;
  m_up = s_up;

  // view space
  m_transform = glm::mat4(1.0f);
  m_transform = glm::lookAt(
      s_position, s_position + s_front, s_up);

  glm::mat4 projection = glm::perspective(45.0f, (float)Window::get()->get_width() / (float)Window::get()->get_height(), 0.1f, 100.0f);

  m_uniform.setMatrix("view", m_transform);
  m_uniform.setMatrix("projection", projection);
}