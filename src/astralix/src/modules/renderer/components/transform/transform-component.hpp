#pragma once
#include "ecs/components/component.hpp"
#include "glm/glm.hpp"

namespace astralix {

class TransformComponent : public Component<TransformComponent> {
public:
  TransformComponent(COMPONENT_INIT_PARAMS,
                     glm::vec3 position = glm::vec3(0.0f),
                     glm::vec3 scale = glm::vec3(1.0f),
                     glm::vec3 rotation = glm::vec3(1.0f),
                     float rotation_angle = 0.0f);

  void set_scale(glm::vec3 p_scale);
  void translate(glm::vec3 p_position);
  void start();
  void update();
  void reset_transform();

  glm::vec3 forward();

  glm::vec3 position;
  glm::vec3 scale;
  glm::vec3 rotation;
  bool m_dirty;
  float rotation_angle;
  glm::mat4 matrix;

  void set_position(glm::vec3 new_pos);

  void set_rotation(glm::vec3 axis, float degrees);

private:
  void recalculate_transform();
};

} // namespace astralix
