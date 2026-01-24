#pragma once
#include "components/component.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

namespace astralix {

class TransformComponent : public Component<TransformComponent> {
public:
  TransformComponent(COMPONENT_INIT_PARAMS,
                     glm::vec3 position = glm::vec3(0.0f),
                     glm::vec3 scale = glm::vec3(1.0f),
                     glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

  void set_scale(glm::vec3 p_scale);
  void translate(glm::vec3 p_position);

  void rotate(glm::vec3 axis, float degrees);
  void rotate(glm::quat rotation);

  void start();
  void update();
  void reset_transform();

  glm::vec3 forward();

  glm::vec3 position;
  glm::vec3 scale;
  glm::quat rotation;
  bool m_dirty;
  glm::mat4 matrix;

  void set_position(glm::vec3 new_pos);

  void recalculate_transform();
};

} // namespace astralix
