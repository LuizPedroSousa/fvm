#pragma once
#include "GLFW/glfw3.h"
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "functional"

class Window {

  public:
  static Window *get();

  Either<BaseException, Unit> open(int width, int height);

  static int get_width() { return get()->m_width; }
  static int get_height() { return get()->m_height; }

  void on_update();
  void on_next();

  static GLFWwindow *get_value();

  bool is_open();
  void close();

  protected:
  Window();

  private:
  static void resizing(GLFWwindow *window, int width, int height);
  static void handle_errors(int, const char *description);
  static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
  static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
  void clear_buffers();
  GLFWwindow *m_value;
  static Window *m_instance;

  int m_height;
  int m_width;
};
