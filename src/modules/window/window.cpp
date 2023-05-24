#include "glad/glad.h"

#include "application.hpp"
#include "either.hpp"
#include "events/key-event.hpp"
#include "events/mouse-event.hpp"
#include "exceptions/base-exception.hpp"
#include "imgui/imgui.h"
#include "stdio.h"
#include "window.hpp"

Window *Window::m_instance = nullptr;

void Window::handle_errors(int, const char *description) {
  std::cout << description << std::endl;
}

Window *Window::get() {
  if (m_instance == nullptr) {
    m_instance = new Window;
  }

  return m_instance;
}

Window::Window() : m_clear_color(glm::vec4(0.5f, 0.5f, 1.0f, 0.0f)) {
  glfwSetErrorCallback(handle_errors);
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow *Window::get_value() {
  return Window::get()->m_value;
}

void Window::clear_buffers() {
  glClearColor(m_clear_color.x, m_clear_color.y, m_clear_color.z, m_clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::resizing(GLFWwindow *window, int width, int height) {
  Window::get()->m_width = width;
  Window::get()->m_height = height;
  glViewport(0, 0, width, height);
}

static bool has_pressed = false;

Either<BaseException, Unit> Window::open(int width, int height) {
  m_width = width;
  m_height = width;
  GLFWwindow *window = glfwCreateWindow(m_width, m_height, "FVM", NULL, NULL);
  m_value = window;

  if (window == NULL) {
    glfwTerminate();

    ASSERT(true, "Couldn't create window for OpenGL");
  }

  glfwMakeContextCurrent(m_value);

  ASSERT(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Couldn't load GLAD");

  glViewport(0, 0, m_width, m_height);

  glfwSetFramebufferSizeCallback(m_value, resizing);

  glEnable(GL_DEPTH_TEST);

  glfwSetInputMode(m_value, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  KeyReleasedDispatcher::get()->attach(KeyReleasedEvent(KeyCode::Escape), []() {
    has_pressed = !has_pressed;

    glfwSetCursorPos(Window::get_value(), Window::get_width() / 2.0f, Window::get_height() / 2.0f);

    glfwSetInputMode(Window::get_value(), GLFW_CURSOR, has_pressed ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
  });

  glfwSetCursorPosCallback(m_value, mouse_callback);
  glfwSetKeyCallback(m_value, key_callback);

  return Unit();
}

void Window::mouse_callback(GLFWwindow *window, double x, double y) {
  if (!has_pressed)
    MouseDispatcher::get()->dispatch(MouseEvent(x, y));
};

void Window::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  for (int i = 0; i < KeyReleasedDispatcher::get()->m_listeners.size(); i++) {
    if (KeyReleasedDispatcher::get()->m_listeners[i].event.get_key() == key && action == GLFW_PRESS) {
      KeyReleasedDispatcher::get()->dispatch(i);
      break;
    }
  };
}

void Window::update() {
  glfwPollEvents();

  for (int i = 0; i < KeyPressedDispatcher::get()->m_listeners.size(); i++) {
    if (glfwGetKey(m_value, KeyPressedDispatcher::get()->m_listeners[i].event.get_key())) {
      KeyPressedDispatcher::get()->dispatch(i);
    }
  };

  clear_buffers();
}

void Window::post_update() {
  glfwSwapBuffers(m_value);
}

void Window::close() {
  delete m_instance;
  glfwTerminate();
}

bool Window::is_open() {
  return !glfwWindowShouldClose(m_value);
}