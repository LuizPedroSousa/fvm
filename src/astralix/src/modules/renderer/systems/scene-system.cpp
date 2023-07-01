#include "scene-system.hpp"
#include "components/camera/camera-component.hpp"
#include "components/light/light-component.hpp"
#include "components/resource/resource-component.hpp"
#include "either.hpp"
#include "engine.hpp"
#include "entities/object.hpp"
#include "managers/scene-manager.hpp"

namespace astralix {

void SceneSystem::start() {
  auto scene = SceneManager::get()->get_active_scene();

  if (scene != nullptr) {
    scene->start();
  }
}

void SceneSystem::fixed_update(double fixed_dt){

};

void SceneSystem::pre_update(double dt){

};

void SceneSystem::update(double dt) {
  auto scene = SceneManager::get()->get_active_scene();

  if (scene != nullptr) {

    scene->update();
  }
};

void SceneSystem::post_update(double dt){};

void SceneSystem::pre_render(Uniform *uniform) {}

} // namespace astralix