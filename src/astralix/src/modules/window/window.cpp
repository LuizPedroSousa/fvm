#include "glad/glad.h"

#include "application.hpp"
#include "either.hpp"
#include "events/key-event.hpp"
#include "events/mouse-event.hpp"
#include "exceptions/base-exception.hpp"
#include "imgui/imgui.h"
#include "stdio.h"
#include "window.hpp"

namespace astralix {

Window *Window::m_instance = nullptr;

void Window::handle_errors(int, const char *description) {
  std::cout << description << std::endl;
}

void Window::init() {
  if (m_instance == nullptr) {
    m_instance = new Window;
  }
}

Window *Window::get() { return m_instance; }

Window::Window() {
  glfwSetErrorCallback(handle_errors);
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow *Window::get_value() { return Window::get()->m_value; }

void Window::resizing(GLFWwindow *window, int width, int height) {
  Window::get()->m_width = width;
  Window::get()->m_height = height;
  glViewport(0, 0, width, height);
}

static bool has_pressed = false;

int Window::get_height() { return m_height; }
int Window::get_width() { return m_width; }

void Window::open(int width, int height) {
  m_width = width;
  m_height = width;
  GLFWwindow *window = glfwCreateWindow(m_width, m_height, "FVM", NULL, NULL);
  m_value = window;

  if (window == NULL) {
    glfwTerminate();

    ASSERT_THROW(true, "Couldn't create window for OpenGL");
  }

  glfwMakeContextCurrent(m_value);

  ASSERT_THROW(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress),
               "Couldn't load GLAD");

  glViewport(0, 0, m_width, m_height);

  glfwSetFramebufferSizeCallback(m_value, resizing);

  KeyReleasedDispatcher::get()->attach(KeyReleasedEvent(KeyCode::Escape), []() {
    has_pressed = !has_pressed;

    auto window = Window::get();

    glfwSetCursorPos(window->get_value(), window->get_width() / 2.0f,
                     window->get_height() / 2.0f);

    glfwSetInputMode(window->get_value(), GLFW_CURSOR,
                     has_pressed ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
  });

  glfwSetCursorPosCallback(m_value, mouse_callback);
  glfwSetKeyCallback(m_value, key_callback);
}

void Window::mouse_callback(GLFWwindow *window, double x, double y) {
  if (!has_pressed)
    MouseDispatcher::get()->dispatch(MouseEvent(x, y));
};

void Window::key_callback(GLFWwindow *window, int key, int scancode, int action,
                          int mods) {
  for (int i = 0; i < KeyReleasedDispatcher::get()->m_listeners.size(); i++) {
    if (KeyReleasedDispatcher::get()->m_listeners[i].event.get_key() == key &&
        action == GLFW_PRESS) {
      KeyReleasedDispatcher::get()->dispatch(i);
      break;
    }
  };
}

void Window::update() {
  glfwPollEvents();

  for (int i = 0; i < KeyPressedDispatcher::get()->m_listeners.size(); i++) {
    if (glfwGetKey(
            m_value,
            KeyPressedDispatcher::get()->m_listeners[i].event.get_key())) {
      KeyPressedDispatcher::get()->dispatch(i);
    }
  };
}

void Window::post_update() { glfwSwapBuffers(m_value); }

void Window::close() {
  glfwTerminate();
  delete m_instance;
}

bool Window::is_open() { return !glfwWindowShouldClose(m_value); }

} // namespace astralix