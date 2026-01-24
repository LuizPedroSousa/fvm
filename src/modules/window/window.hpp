#pragma once

#include "GLFW/glfw3.h"
#include "events/key-event.hpp"
#include "guid.hpp"
#include <unordered_map>

namespace astralix {

class Window {

public:
  static void init();
  static Window *get();

  void open(std::string &title, int width, int height, bool offscreen = false);

  int get_width();
  int get_height();
  std::string get_title();

  void update();
  void swap();

  static GLFWwindow *get_value();

  bool is_open();
  void close();

  void destroy_key(int key);

protected:
  Window();

private:
  static void resizing(GLFWwindow *window, int width, int height);
  static void handle_errors(int, const char *description);
  static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
  static void key_callback(GLFWwindow *window, int key, int scancode,
                           int action, int mods);
  static void toggle_view_mouse(KeyReleasedEvent *event);

  GLFWwindow *m_value;
  static Window *m_instance;

  int m_height = 0;
  int m_width = 0;
  std::string m_title;
  bool m_offscreen;
};

#define WINDOW_WIDTH() Window::get()->get_width()
#define WINDOW_HEIGHT() Window::get()->get_height()
} // namespace astralix
