#pragma once
#include "ecs/components/component.hpp"
#include "glm/glm.hpp"
#include "resources/shader.hpp"
#include "uniform.hpp"

namespace astralix {

class CameraComponent : public Component<CameraComponent> {
public:
  CameraComponent(COMPONENT_INIT_PARAMS, glm::vec3 position);
  CameraComponent();

  void update(Uniform *uniform);

  void use_perspective();
  void use_orthographic();

  glm::vec3 get_position() { return m_position; };
  glm::vec3 get_front() { return m_front; };

  glm::mat4 get_view_matrix() { return m_view_matrix; }
  glm::mat4 get_projection_matrix() { return m_projection_matrix; }

private:
  void recalculate_projection_matrix();
  void recalculate_view_matrix();

  glm::vec3 m_up;
  glm::vec3 m_front;
  unsigned int m_shader_render_id;
  glm::vec3 m_position;

  glm::mat4 m_view_matrix;
  glm::mat4 m_projection_matrix;

  bool m_is_orthographic;
};

} // namespace astralix