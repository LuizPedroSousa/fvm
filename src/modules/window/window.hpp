#pragma once
#include "GLFW/glfw3.h"
#include "either.hpp"
#include "exceptions/base-exception.hpp"

class Window
{
private:
  GLFWwindow *value;
  static void resizing(GLFWwindow *window, int width, int height);
  static void handleErrors(int, const char *description);

public:
  Window();

  void process_input();
  void clear_buffers();
  Either<BaseException, Unit> open();
  void close();

  GLFWwindow *get();
};
