#include "render-component.hpp"
#include "camera-component.hpp"
#include "game.hpp"
#include "iostream"
#include "light-component.hpp"
#include "material-component.hpp"
#include "mesh-component.hpp"
#include "resource-component.hpp"
#include "transform-component.hpp"

#include "glad/glad.h"

RenderComponent::RenderComponent(COMPONENT_INIT_PARAMS) : COMPONENT_INIT(RenderComponent) {
}

void RenderComponent::start(Object *owner) {
  if (owner->is_active()) {
    auto resource = owner->get_component<ResourceComponent>();
    auto transform = owner->get_component<TransformComponent>();
    auto mesh = owner->get_component<MeshComponent>();

    mesh->start();

    resource->start();
    transform->start();
  };
}

void RenderComponent::pre_update(Object *owner) {
  if (owner->is_active()) {
  }
}

void RenderComponent::update(Object *owner) {

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
