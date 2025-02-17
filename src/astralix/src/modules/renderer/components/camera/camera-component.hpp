#pragma once
#include "ecs/components/component.hpp"
#include "glm/glm.hpp"
#include "resources/shader.hpp"

namespace astralix {

class CameraComponent : public Component<CameraComponent> {
public:
  CameraComponent(COMPONENT_INIT_PARAMS);

  void update(Ref<Shader> &shader);

  void use_perspective();
  void use_orthographic();

  glm::mat4 get_view_matrix() { return m_view_matrix; }
  glm::mat4 get_projection_matrix() { return m_projection_matrix; }

  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 front = glm::vec3(0.003012, -0.509046, 0.860734);
  glm::vec3 rotation;

  glm::vec3 direction;

  float yaw = -90.0f;
  float pitch = 0.0f;

  float last_offset_x, last_offset_y;

  float offset_x, offset_y;

  bool is_first_recalculation = true;

  float speed = 5.0f;
  float sensitivity = 0.1f;

  void recalculate_camera_rotation(float x, float y);

private:
  void recalculate_projection_matrix();
  void recalculate_view_matrix();

  glm::mat4 m_view_matrix;
  glm::mat4 m_projection_matrix;

  bool m_is_orthographic;
};

} // namespace astralix
