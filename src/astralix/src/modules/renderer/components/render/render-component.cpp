#include "render-component.hpp"
#include "components/camera/camera-component.hpp"
#include "components/light/light-component.hpp"
#include "components/material/material-component.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/resource/resource-component.hpp"
#include "components/transform/transform-component.hpp"
#include "engine.hpp"
#include "iostream"

#include "glad/glad.h"

namespace astralix {

RenderComponent::RenderComponent(COMPONENT_INIT_PARAMS)
    : COMPONENT_INIT(RenderComponent) {}

void RenderComponent::start(IEntity *owner) {
  if (owner->is_active()) {
    auto resource = owner->get_component<ResourceComponent>();
    auto transform = owner->get_component<TransformComponent>();
    auto mesh = owner->get_component<MeshComponent>();

    mesh->start();

    resource->start();
    if (transform != nullptr)
      transform->start();
  };
}

void RenderComponent::pre_update(IEntity *owner) {
  if (owner->is_active()) {
  }
}

void RenderComponent::update(IEntity *owner) {
  if (m_draw_callbacks.before)
    m_draw_callbacks.before();

  if (owner->is_active()) {
    auto transform = owner->get_component<TransformComponent>();
    auto material = owner->get_component<MaterialComponent>();
    auto resource = owner->get_component<ResourceComponent>();
    auto mesh = owner->get_component<MeshComponent>();

    resource->pre_update();

    if (transform != nullptr) {
      transform->update();
    }

    if (material != nullptr) {
      material->update();
    }

    mesh->draw();

    if (m_draw_callbacks.after)
      m_draw_callbacks.after();
  };
}

} // namespace astralix