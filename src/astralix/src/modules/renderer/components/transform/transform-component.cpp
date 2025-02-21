#include "transform-component.hpp"
#include "components/resource/resource-component.hpp"
#include "components/transform/serializers/transform-component-serializer.hpp"
#include "engine.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "iostream"

namespace astralix {

TransformComponent::TransformComponent(COMPONENT_INIT_PARAMS,
                                       glm::vec3 position, glm::vec3 scale,
                                       glm::vec3 rotation, float rotation_angle)
    : COMPONENT_INIT(TransformComponent, "Transform", false,
                     create_ref<TransformComponentSerializer>(this)),
      position(position), scale(scale), rotation(rotation),
      rotation_angle(rotation_angle), m_dirty(true) {}

void TransformComponent::start() { recalculate_transform(); }

void TransformComponent::recalculate_transform() {
  if (!m_dirty)
    return;

  if (glm::length(rotation) == 0.0f) {
    rotation = glm::vec3(0.0f, 0.0f, 1.0f);
  }

  glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position);
  glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), scale);
  glm::mat4 rotatation_matrix =
      glm::rotate(glm::mat4(1.0f), glm::radians(rotation_angle), rotation);

  matrix = translation_matrix * rotatation_matrix * scale_matrix;
}

void TransformComponent::update() {
  recalculate_transform();

  auto owner = EntityManager::get()->get_entity(get_owner_id());

  auto resource = owner->get_component<ResourceComponent>();

  if (resource != nullptr && resource->has_shader()) {
    resource->get_shader()->set_matrix("model", this->matrix);
  }
}

void TransformComponent::translate(glm::vec3 p_position) {
  this->matrix = glm::translate(this->matrix, p_position);
  this->position = p_position;

  m_dirty = true;
}

void TransformComponent::set_scale(glm::vec3 p_scale) {
  this->matrix = glm::scale(this->matrix, p_scale);
  this->scale = p_scale;

  m_dirty = true;
}

glm::vec3 TransformComponent::forward() {
  return glm::normalize(glm::vec3(matrix[2]));
}

void TransformComponent::reset_transform() {
  this->position = glm::vec3(0.0f);
  this->scale = glm::vec3(1.0f);
  this->rotation = glm::vec3(0.0f, 0.0f, 1.0f);
  this->rotation_angle = 0.0f;
  this->matrix = glm::mat4(1.0f);
  m_dirty = true;
  recalculate_transform();
}
void TransformComponent::set_position(glm::vec3 new_pos) {
  position = new_pos;
  m_dirty = true;
}

void TransformComponent::rotate(glm::vec3 axis, float degrees) {
  rotation = glm::length(axis) > 0 ? glm::normalize(axis)
                                   : glm::vec3(0.0f, 0.0f, 1.0f);
  rotation_angle = degrees;
  m_dirty = true;
}

} // namespace astralix
