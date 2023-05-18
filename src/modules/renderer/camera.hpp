#pragma once
#include "glm/glm.hpp"
#include "shader.hpp"
#include "uniform.hpp"

class Camera {
  public:
  Camera();

  Camera(unsigned int shader_render_id, glm::vec3 position);

  void render();

  private:
  glm::vec3 m_up;
  glm::vec3 m_front;
  unsigned int m_shader_render_id;
  glm::mat4 m_transform;
  glm::vec3 m_position;
  Uniform m_uniform;
};