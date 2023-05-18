#include "prologue.hpp"
#include "imgui/imgui.h"
#include "iostream"
#include <glm/gtx/string_cast.hpp>

void Prologue::start() {
  auto s_attached = m_shader_renderer.attach(Shader::create("shaders/vertex/triangle.glsl", "shaders/fragment/triangle.glsl"));

  if (s_attached.isLeft()) {
    throw s_attached.left();
  };

  auto t_attached = m_texture_renderer.attach(
      Texture::create("block_texture", "block.png", GL_RGBA, m_shader_renderer.shaders[0]));

  if (t_attached.isLeft()) {
    throw t_attached.left();
  }

  m_camera = Camera(m_shader_renderer.id, glm::vec3(0.0f, 0.0f, 3.0f));

  for (int j = 0; j < 10; j++) {
    for (int i = 1; i < 10; i++) {
      m_object_renderer.spawn(Object::cube(glm::vec3(0.5f * i, 0.0f, 0.5f * j), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 0.0f, m_shader_renderer, m_texture_renderer));
    }
  }

  m_object_renderer.start_all();
}

void Prologue::update() {
  m_camera.render();
  m_object_renderer.render_all();
}