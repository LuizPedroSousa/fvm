#include "transform-component.hpp"
#include "glm/gtc/matrix_transform.hpp"

TransformComponent::TransformComponent(COMPONENT_INIT_PARAMS) : COMPONENT_INIT(TransformComponent) {
  m_initial_transform = glm::mat4(1.0f);
}

void TransformComponent::transform_initial() {
  m_render_transform = m_initial_transform;
}

void TransformComponent::transform() {
  m_render_transform = m_initial_transform;
}

void TransformComponent::translate_initial(glm::vec3 position) {
  m_initial_transform = glm::translate(m_initial_transform, position);
  m_position = position;
}

void TransformComponent::translate(glm::vec3 position) {
  m_position = position;
  m_render_transform = glm::translate(m_render_transform, position);
}

void TransformComponent::scale_initial(glm::vec3 scale) {
  m_initial_transform = glm::scale(m_initial_transform, scale);
}

void TransformComponent::scale(glm::vec3 scale) {
  m_render_transform = glm::scale(m_render_transform, scale);
}