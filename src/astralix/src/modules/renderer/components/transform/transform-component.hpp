#pragma once
#include "ecs/components/component.hpp"
#include "glm/glm.hpp"

namespace astralix {

struct Transform {
  glm::vec3 position;
  glm::vec3 scale;
  glm::vec3 rotation;
  float rotation_angle;

  glm::mat4 matrix;
};

class TransformComponent : public Component<TransformComponent> {
public:
  TransformComponent(COMPONENT_INIT_PARAMS, glm::vec3 position,
                     glm::vec3 scale);

  TransformComponent(){};

  void translate_initial(glm::vec3 position);
  void scale_initial(glm::vec3 scale);

  void scale(glm::vec3 scale);
  void translate(glm::vec3 position);

  void start();
  void update();

  Transform *get_render_transform() { return &m_render_transform; }

  Transform *get_initial_transform() { return &m_initial_transform; }

  void reset_render_transform();

private:
  Transform m_initial_transform;
  Transform m_render_transform;
};

} // namespace astralix