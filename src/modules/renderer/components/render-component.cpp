#include "render-component.hpp"
#include "camera-component.hpp"
#include "components/light-component.hpp"
#include "components/resource-component.hpp"
#include "game.hpp"
#include "iostream"

#include "glad/glad.h"

RenderComponent::RenderComponent(COMPONENT_INIT_PARAMS) : COMPONENT_INIT(RenderComponent) {
}

void RenderComponent::start() {
  auto manager = Game::get()->get_entity_manager();
  Object *owner = static_cast<Object *>((manager->get_entity(get_owner_id())));
  if (owner->is_active()) {
    auto resource = owner->get_component<ResourceComponent>();
    create_buffers(owner);
    resource->start();
    owner->start();
  };
}

void RenderComponent::update() {
  auto manager = Game::get()->get_entity_manager();
  Object *owner = static_cast<Object *>((manager->get_entity(get_owner_id())));

  if (owner->is_active()) {
    auto resource = owner->get_component<ResourceComponent>();
    resource->update();

    auto light = owner->get_component<LightComponent>();

    if (light != nullptr) {
      light->update(resource->get_shader_renderer()->get_uniform());
    }

    resource->update();

    owner->update();
    draw(owner);
  };
}

void RenderComponent::draw(Object *object) {
  glBindVertexArray(object->m_buffers.VAO);
  glDrawArrays(GL_TRIANGLES, 0, object->vertices.size);
}

void RenderComponent::create_buffers(Object *object) {
  GLuint VAO, VBO;

  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, object->vertices.size * 8 * sizeof(float), object->vertices.vertex, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  object->m_buffers = {
      .VAO = VAO,
      .VBO = VBO,
  };
}