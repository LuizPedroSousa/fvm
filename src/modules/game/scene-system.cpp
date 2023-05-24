#include "scene-system.hpp"
#include "components/camera-component.hpp"
#include "either.hpp"
#include "game.hpp"
#include "scenes/prologue.hpp"

Either<BaseException, Unit> SceneSystem::start() {
  auto entity = Game::get()->get_entity_manager()->add_entity<Prologue>();

  m_scenes.push_back(entity.get_entity_id());

  auto has_started = ((Prologue *)get_current_scene())->start();

  ASSERT_COMPARE(has_started);

  return Unit();
}

IEntity *SceneSystem::get_current_scene() {
  return Game::get()->get_entity_manager()->get_entity(m_scenes[m_current_scene]);
}

void SceneSystem::fixed_update(double fixed_dt){

};

void SceneSystem::pre_update(double dt){

};

void SceneSystem::update(double dt) {
  auto scene = dynamic_cast<Scene *>(get_current_scene());

  scene->update();
};

void SceneSystem::post_update(double dt){};

void SceneSystem::post_render(Uniform *uniform) {
  auto camera = get_current_scene()->get_component<CameraComponent>();

  if (camera != nullptr) {
    camera->update(uniform);
  }
}