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
    : COMPONENT_INIT(
      TransformComponent, "Transform", false,
      create_ref<TransformComponentSerializer>(this)),
    position(position), scale(scale), rotation(rotation),
    rotation_angle(rotation_angle) {
  }

  void TransformComponent::start() {}

  void TransformComponent::recalculate_transform() {
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
  }

  void TransformComponent::set_scale(glm::vec3 p_scale) {
    this->matrix = glm::scale(this->matrix, p_scale);
    this->scale = p_scale;
  }

  void TransformComponent::reset_transform() {
    this->position = glm::vec3(0.0f);
    this->scale = glm::vec3(1.0f);
    this->rotation = glm::vec3(1.0f, 1.0f, 1.0f);
    this->rotation_angle = 0.0f;
    this->matrix = glm::mat4(1.0f);
  }

} // namespace astralix