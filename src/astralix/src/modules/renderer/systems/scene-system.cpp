#include "scene-system.hpp"
#include "components/camera/camera-component.hpp"
#include "components/light/light-component.hpp"
#include "components/resource/resource-component.hpp"
#include "either.hpp"
#include "engine.hpp"
#include "entities/object.hpp"

namespace astralix {

void SceneSystem::start() {
  auto scene = get_current_scene();

  if (scene == nullptr) {
    return;
  }

  auto light = scene->get_component<LightComponent>();

  if (light != nullptr) {
    light->start();
  }

  scene->start();
}

void SceneSystem::fixed_update(double fixed_dt){

};

void SceneSystem::pre_update(double dt){

};

void SceneSystem::update(double dt) {
  auto scene = get_current_scene();

  if (scene != nullptr) {
    auto manager = Engine::get()->get_entity_manager();

    auto light  = scene->get_component<LightComponent>();
    auto camera = scene->get_component<CameraComponent>();

    manager->for_each<Object>([&](Object *object) {
      auto resource = object->get_component<ResourceComponent>();

      resource->get_shader_renderer()->use();

      if (light != nullptr) {
        light->update(object);
      }

      if (camera != nullptr) {
        camera->update(resource->get_shader_renderer_uniform());
      }
    });

    scene->update();
  }
};

void SceneSystem::post_update(double dt){};

void SceneSystem::pre_render(Uniform *uniform) {}

Scene *SceneSystem::get_current_scene() {
  auto manager = Engine::get()->get_entity_manager();

  auto scenes = manager->get_entities<Scene>();

  if (scenes.size() == 0) {
    return nullptr;
  }

  return scenes[m_current_scene];
}

} // namespace astralix