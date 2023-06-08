#pragma once
#include "components/base/component.hpp"
#include "glm/glm.hpp"
#include "resources/shader.hpp"
#include "uniform.hpp"

class CameraComponent : public Component<CameraComponent> {
  public:
  CameraComponent(COMPONENT_INIT_PARAMS, glm::vec3 position);
  CameraComponent();

  void update(Uniform *uniform);

  void use_perspective();
  void use_orthographic();

  glm::vec3 get_position() { return m_position; };
  glm::vec3 get_front() { return m_front; };

  private:
  glm::mat4 get_projection();
  glm::mat4 get_view();

  glm::vec3 m_up;
  glm::vec3 m_front;
  unsigned int m_shader_render_id;
  glm::vec3 m_position;

  bool m_is_orthographic;
};