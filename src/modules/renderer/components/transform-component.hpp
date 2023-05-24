#pragma once
#include "components/base/component.hpp"
#include "glm/glm.hpp"

class TransformComponent : public Component<TransformComponent> {
  public:
  TransformComponent(COMPONENT_INIT_PARAMS);

  TransformComponent(){};

  void translate_initial(glm::vec3 position);
  void transform_initial();
  void scale(glm::vec3 scale);
  void scale_initial(glm::vec3 scale);
  void translate(glm::vec3 position);
  void transform();

  glm::vec3 *get_position() {
    return &m_position;
  }

  glm::vec3 *get_scale() {
    return &m_scale;
  }

  glm::vec3 *get_rotation() {
    return &m_scale;
  }

  glm::vec3 *get_rotation_angle() {
    return &m_scale;
  }

  glm::mat4 get_render_transform() {
    return m_render_transform;
  }

  glm::mat4 get_initial_transform() {
    return m_initial_transform;
  }

  private:
  glm::vec3 m_initial_position;

  glm::mat4 m_initial_transform;
  glm::mat4 m_render_transform;

  glm::vec3 m_position;
  glm::vec3 m_scale;
  glm::vec3 m_rotation;
  glm::vec3 m_rotation_angle;
};