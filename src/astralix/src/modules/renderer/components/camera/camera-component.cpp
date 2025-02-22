#include "camera-component.hpp"
#include "components/transform/transform-component.hpp"

#include "window.hpp"

#include "components/camera/serializers/camera-component-serializer.hpp"
#include "engine.hpp"
#include "events/event-dispatcher.hpp"

namespace astralix {

CameraComponent::CameraComponent(COMPONENT_INIT_PARAMS)
    : COMPONENT_INIT(CameraComponent, "Camera", true,
                     create_ref<CameraComponentSerializer>(this)),
      m_is_orthographic(false) {

  auto event_dispatcher = EventDispatcher::get();
}

void CameraComponent::use_orthographic() { m_is_orthographic = true; }

void CameraComponent::use_perspective() { m_is_orthographic = false; }

void CameraComponent::recalculate_projection_matrix() {
  if (m_is_orthographic) {
    m_projection_matrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
  } else {

    auto window = Window::get();
    const auto &spec = Engine::get()->framebuffer->get_specification();

    m_projection_matrix = glm::perspective(
        45.0f, (float)spec.width / (float)spec.height, 0.1f, 100.0f);
  }
}

void CameraComponent::recalculate_view_matrix() {
  // view space
  auto matrix = glm::mat4(1.0f);

  auto transform = get_owner()->get_component<TransformComponent>();

  matrix = glm::lookAt(transform->position, transform->position + front, up);

  m_view_matrix = matrix;
}

void CameraComponent::update(Ref<Shader> &shader) {
  recalculate_view_matrix();
  recalculate_projection_matrix();

  auto transform = get_owner()->get_component<TransformComponent>();

  shader->set_matrix("view", m_view_matrix);
  shader->set_vec3("view_position", transform->position);
  shader->set_matrix("projection", m_projection_matrix);
}

} // namespace astralix
