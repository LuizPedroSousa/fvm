#pragma once
#include "GLFW/glfw3.h"
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "functional"
#include "glm/glm.hpp"

namespace astralix {

class Window {

public:
  static void init();
  static Window *get();

  void open(const char *title, int width, int height);

  int get_width();
  int get_height();
  std::string get_title();

  void update();
  void post_update();

  static GLFWwindow *get_value();

  bool is_open();
  void close();

protected:
  Window();

private:
  static void resizing(GLFWwindow *window, int width, int height);
  static void handle_errors(int, const char *description);
  static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
  static void key_callback(GLFWwindow *window, int key, int scancode,
                           int action, int mods);
  static void toggle_view_mouse();

  GLFWwindow *m_value;
  static Window *m_instance;

  int m_height = 0;
  int m_width = 0;
  std::string m_title;
};

} // namespace astralix