#include "glad/glad.h"
#include "window.hpp"
#include "stdio.h"
#include "either.hpp"
#include "exceptions/base-exception.hpp"

void Window::handleErrors(int, const char *description)
{
  std::cout << description << std::endl;
}

Window::Window()
{
  glfwSetErrorCallback(handleErrors);
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow *Window::get()
{
  return this->value;
}

void Window::clear_buffers()
{
  glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::resizing(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

Either<BaseException, Unit> Window::open()
{
  GLFWwindow *window = glfwCreateWindow(800, 600, "FVM", NULL, NULL);
  this->value = window;

  if (window == NULL)
  {

    glfwTerminate();

    return BaseException("Couldn't create window for OpenGL");
  }

  glfwMakeContextCurrent(this->get());

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    return BaseException("Couldn't load GLAD");
  }

  glViewport(0, 0, 800, 600);

  glfwSetFramebufferSizeCallback(this->get(), resizing);

  glEnable(GL_DEPTH_TEST);

  return Unit();
}

void Window::close()
{
  glfwTerminate();
}

void Window::process_input()
{
}