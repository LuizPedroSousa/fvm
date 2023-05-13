#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <cmath>
#include <stdio.h>
#include "modules/window/window.hpp"
#include "shared/either.hpp"
#include "shared/exceptions/base-exception.hpp"
#include "vector"
#include "modules/renderer/shader-renderer.hpp"
#include "modules/renderer/object-renderer.hpp"
#include "modules/renderer/object.hpp"
#include "modules/renderer/shader.hpp"
#include "tuple"
#include "iostream"
#include "modules/renderer/uniform.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

int handleException(BaseException exception)
{
  std::cout << exception.message << std::endl;

  return -1;
}

int main(int argc, char **argv)
{
  Window window;

  auto hasOpened = window.open();

  if (hasOpened.isLeft())
  {
    return handleException(hasOpened.left());
  }

  ObjectRenderer o_renderer;
  ShaderRenderer s_renderer;
  TextureRenderer t_renderer;

  auto s_attached = s_renderer.attach(Shader::create("shaders/vertex/triangle.glsl", "shaders/fragment/triangle.glsl"));

  if (s_attached.isLeft())
  {
    return handleException(s_attached.left());
  };

  auto t_attached = t_renderer.attachMany(2,
                                          Texture::createMany(2, {{"block_texture", "block.png", GL_RGBA, s_renderer.shaders[0]},
                                                                  {"container_texture", "container.jpg", GL_RGB, s_renderer.shaders[0]}}));

  if (t_attached.isLeft())
  {
    return handleException(t_attached.left());
  }

  glm::vec3 positions[] = {
      glm::vec3(1, 1.0f, 0.0f),
      glm::vec3(0.0f, 1.0f, 0.0f),
      glm::vec3(-1, 1.0f, 0.0f),
      glm::vec3(-1, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(1, 0.0f, 0.0f),
      glm::vec3(1, -1.0f, 0.0f),
      glm::vec3(0.0f, -1.0f, 0.0f),
      glm::vec3(-1, -1.0f, 0.0f),
  };

  for (int i = 0; i < sizeof(positions) / sizeof(positions[0]); i++)
  {
    o_renderer.spawn(Object::cube(positions[i], s_renderer, t_renderer));
  }

  o_renderer.start_all();

  while (!glfwWindowShouldClose(window.get()))
  {
    window.process_input();

    window.clear_buffers();

    o_renderer.render_all();

    glfwSwapBuffers(window.get());
    glfwPollEvents();
  }

  window.close();

  return 0;
}