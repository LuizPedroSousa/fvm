#include "assert.hpp"
#include "ecs/guid.hpp"
#include "events/event-scheduler.hpp"

#include "events/key-codes.hpp"
#include "glad/glad.h"

#include "base.hpp"
#include "engine.hpp"
#include "events/event-dispatcher.hpp"
#include "events/key-event.hpp"
#include "events/mouse-event.hpp"
#include "iostream"
#include "stdio.h"
#include "window.hpp"
#include <GLFW/glfw3.h>

namespace astralix {

  Window* Window::m_instance = nullptr;

  void Window::handle_errors(int, const char* description) {
    std::cout << description << std::endl;
  }

  void Window::init() {

    if (m_instance == nullptr) {
      m_instance = new Window;
    }
  }

  Window* Window::get() { return m_instance; }

  Window::Window() {
    glfwSetErrorCallback(handle_errors);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto engine = Engine::get();
    if (engine->has_msaa_enabled())
      glfwWindowHint(GLFW_SAMPLES, engine->msaa.samples);
  }

  GLFWwindow* Window::get_value() { return Window::get()->m_value; }

  void Window::resizing(GLFWwindow* window, int width, int height) {
    Window::get()->m_width = width;
    Window::get()->m_height = height;
    glViewport(0, 0, width, height);
  }

  static bool has_pressed = false;

  int Window::get_height() { return m_height; }
  int Window::get_width() { return m_width; }
  std::string Window::get_title() { return m_title; }

  void Window::open(const char* title, int width, int height, bool offscreen) {
    m_width = width;
    m_height = height;
    m_title = title;
    m_offscreen = offscreen;

    if (offscreen) {
      glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    }

    GLFWwindow* window = glfwCreateWindow(m_width, m_height, title, NULL, NULL);
    m_value = window;

    if (window == NULL) {
      glfwTerminate();

      ASTRA_EXCEPTION(true, "Couldn't create window for OpenGL");
    }

    glfwMakeContextCurrent(m_value);

    ASTRA_EXCEPTION(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress),
      "Couldn't load GLAD");

    glViewport(0, 0, m_width, m_height);

    if (!m_offscreen) {
      glfwSetFramebufferSizeCallback(m_value, resizing);
      EventDispatcher::get()->attach<KeyboardListener, KeyReleasedEvent>(
        ASTRA_BIND_EVENT_FN(Window::toggle_view_mouse));

      glfwSetCursorPosCallback(m_value, mouse_callback);

      glfwSetKeyCallback(m_value, key_callback);
    }
  }

  void Window::toggle_view_mouse(KeyReleasedEvent* event) {
    if (event->key_code == KeyCode::Escape) {
      has_pressed = !has_pressed;

      auto window = Window::get();

      glfwSetCursorPos(window->get_value(), window->get_width() / 2.0f,
        window->get_height() / 2.0f);

      glfwSetInputMode(window->get_value(), GLFW_CURSOR,
        has_pressed ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }
  }

  void Window::mouse_callback(GLFWwindow* window, double x, double y) {
    // std::cout << "x " << x << "\n";
    // std::cout << "y " << y << "\n";
    if (!has_pressed) {
      EventDispatcher::get()->dispatch(new MouseEvent(x, y));
    }
  };

  void Window::attach_key(int key, SchedulerID scheduler_id) {
    auto emplaced = m_key_pressed_scheduler.emplace(key, scheduler_id);
    // ASTRA_EXCEPTION(!emplaced.second, "Error while attaching key pressed");
  }

  void Window::destroy_key(int key) { m_key_pressed_scheduler.erase(key); }

  SchedulerID Window::get_key_scheduler_id(int key) {
    auto key_scheduler = m_key_pressed_scheduler.find(key);

    if (key_scheduler == m_key_pressed_scheduler.end()) {
      return -1;
      // ASTRA_EXCEPTION(true, "Key not found in scheduler");
    }

    return key_scheduler->second;
  }

  void Window::key_callback(GLFWwindow* window, int key, int scancode, int action,
    int mods) {
    auto scheduler = EventScheduler::get();

    switch (action) {
    case GLFW_PRESS: {
      std::cout << "KeyPressed " << key << "\n";
      auto event = KeyCode(key);

      auto scheduler_id =
        scheduler->schedule<KeyPressedEvent>(SchedulerType::POST_FRAME, event);

      Window::get()->attach_key(key, scheduler_id);

      break;
    }
    default:
      break;
    }
  }

  void Window::update() {
    glfwPollEvents();
    auto scheduler = EventScheduler::get();

    auto it = m_key_pressed_scheduler.begin();

    for (; it != m_key_pressed_scheduler.end();) {

      auto state = glfwGetKey(m_value, it->first);

      std::cout << " key " << it->first << " state " << state << "\n";

      if (state == GLFW_RELEASE) {

        if (it->first == 341) {
        }
        std::cout << "KeyReleased " << it->first << "\n";
        auto keycode = KeyReleasedEvent(KeyCode(it->first));

        scheduler->destroy(it->second);

        EventDispatcher::get()->dispatch(&keycode);
        it = m_key_pressed_scheduler.erase(it);
      }
      else {
        it++;
      }
    }
  }

  void Window::swap() { glfwSwapBuffers(m_value); }

  void Window::close() {
    glfwTerminate();
    delete m_instance;
  }

  bool Window::is_open() { return !glfwWindowShouldClose(m_value); }

} // namespace astralix
