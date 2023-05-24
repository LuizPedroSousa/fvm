#include "render-system.hpp"
#include "components/render-component.hpp"
#include "components/resource-component.hpp"
#include "entities/object.hpp"
#include "game.hpp"
#include "iostream"
#include "scene-system.hpp"

RenderSystem::RenderSystem(SceneSystem *scene_system) : m_scene_system(scene_system){};

Either<BaseException, Unit> RenderSystem::start() {

  auto manager = Game::get()->get_entity_manager();
  auto entities = manager->get_entities();

  for (auto entity = entities.begin(); entity != entities.end(); entity++) {
    auto render = entity->second->get_component<RenderComponent>();

    if (render != nullptr && render->is_active()) {
      render->start();
    }
  }

  return Unit();
}

void RenderSystem::fixed_update(double fixed_dt){

};

void RenderSystem::pre_update(double dt){

};

void RenderSystem::update(double dt) {

  auto manager = Game::get()->get_entity_manager();
  auto entities = manager->get_entities();

  auto current_scene = m_scene_system->get_current_scene();

  for (auto entity = entities.begin(); entity != entities.end(); entity++) {
    auto render = entity->second->get_component<RenderComponent>();
    auto resource = entity->second->get_component<ResourceComponent>();

    if (render != nullptr && render->is_active()) {
      render->update();

      m_scene_system->post_render(resource->get_shader_renderer_uniform());
    }
  }
};

void RenderSystem::post_update(double dt){

};