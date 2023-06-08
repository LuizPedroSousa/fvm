#include "render-system.hpp"
#include "components/render-component.hpp"
#include "components/resource-component.hpp"
#include "glad/glad.h"

#include "components/camera-component.hpp"
#include "components/light-component.hpp"
#include "entities/object.hpp"
#include "game.hpp"
#include "iostream"
#include "scene-system.hpp"

RenderSystem::RenderSystem(SceneSystem *scene_system) : m_clear_color(glm::vec4(0.5f, 0.5f, 1.0f, 0.0f)), m_scene_system(scene_system){};

Either<BaseException, Unit> RenderSystem::start() {
  auto manager = Game::get()->get_entity_manager();
  auto entities = manager->get_entities<Object>();

  auto light = m_scene_system->get_current_scene()->get_component<LightComponent>();

  if (light != nullptr) {
    light->start();
  }

  for (auto entity = entities.begin(); entity != entities.end(); entity++) {

    if (entity->second->is_active() && entity->second->get_entity_type_id() == Object::entity_type_id()) {
      Object *owner = static_cast<Object *>(manager->get_entity(entity->first));

      auto render = owner->get_component<RenderComponent>();

      if (render != nullptr && render->is_active()) {
        render->start(owner);
      }
    }
  }

  return Unit();
}

void RenderSystem::fixed_update(double fixed_dt){

};

void RenderSystem::pre_update(double dt) {
  auto manager = Game::get()->get_entity_manager();
  auto entities = manager->get_entities<Object>();

  glClearColor(m_clear_color.x, m_clear_color.y, m_clear_color.z, m_clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);

  for (auto entity = entities.begin(); entity != entities.end(); entity++) {
    Object *owner = static_cast<Object *>(manager->get_entity(entity->first));

    auto resource = owner->get_component<ResourceComponent>();
    auto render = owner->get_component<RenderComponent>();

    if (render != nullptr && render->is_active()) {
      render->pre_update(owner);
    }
  }
};

void RenderSystem::update(double dt) {

  auto manager = Game::get()->get_entity_manager();
  auto entities = manager->get_entities<Object>();

  auto camera = m_scene_system->get_current_scene()->get_component<CameraComponent>();
  auto light = m_scene_system->get_current_scene()->get_component<LightComponent>();

  if (camera == nullptr && light == nullptr) return;

  for (auto entity = entities.begin(); entity != entities.end(); entity++) {
    EntityID id = entity->first;

    auto owner = static_cast<Object *>(manager->get_entity(id));
    auto resource = owner->get_component<ResourceComponent>();

    resource->get_shader_renderer()->use();

    if (light != nullptr) {
      light->update(owner);
    }

    if (camera != nullptr) {
      camera->update(resource->get_shader_renderer_uniform());
    }

    auto render = owner->get_component<RenderComponent>();

    if (render != nullptr && render->is_active()) {
      render->update(owner);
    }
  }
};

void RenderSystem::post_update(double dt){

};