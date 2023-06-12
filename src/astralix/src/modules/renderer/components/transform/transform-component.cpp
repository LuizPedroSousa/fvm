#include "transform-component.hpp"
#include "components/resource/resource-component.hpp"
#include "engine.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "iostream"

namespace astralix {

TransformComponent::TransformComponent(COMPONENT_INIT_PARAMS,
                                       glm::vec3 position, glm::vec3 scale)
    : COMPONENT_INIT(TransformComponent) {
  m_initial_transform = {
      .position = position,
      .scale = scale,
      .rotation = glm::vec3(1.0f, 1.0f, 1.0f),
      .rotation_angle = 0.0f,
      .matrix = glm::mat4(1.0f),
  };
}

void TransformComponent::start() { m_render_transform = m_initial_transform; }

void TransformComponent::update() {
  m_render_transform.matrix = m_initial_transform.matrix;

  m_render_transform.matrix =
      glm::translate(m_render_transform.matrix, m_render_transform.position);
  m_render_transform.matrix =
      glm::scale(m_render_transform.matrix, m_render_transform.scale);
  m_render_transform.matrix =
      glm::rotate(m_render_transform.matrix,
                  glm::radians(m_render_transform.rotation_angle),
                  m_render_transform.rotation);

  auto owner = Engine::get()->get_entity_manager()->get_entity(get_owner_id());

  auto resource = owner->get_component<ResourceComponent>();

  if (resource != nullptr) {
    resource->get_shader_renderer()->get_uniform()->setMatrix(
        "model", m_render_transform.matrix);
  }
}

void TransformComponent::translate_initial(glm::vec3 position) {
  m_initial_transform.position = position;
}

void TransformComponent::scale_initial(glm::vec3 scale) {
  m_initial_transform.scale = scale;
}

void TransformComponent::translate(glm::vec3 position) {
  m_render_transform.matrix =
      glm::translate(m_render_transform.matrix, position);
  m_render_transform.position = position;
}

void TransformComponent::scale(glm::vec3 scale) {
  m_render_transform.matrix = glm::scale(m_render_transform.matrix, scale);
  m_render_transform.scale = scale;
}

void TransformComponent::reset_render_transform() {
  m_render_transform = m_initial_transform;
}

} // namespace astralix