#include "render-system.hpp"
#include "ecs/managers/entity-manager.hpp"
#include "events/event-scheduler.hpp"
#include "glad/glad.h"

#include "engine.hpp"
#include "entities/object.hpp"
#include "entities/post-processing.hpp"
#include "iostream"
#include "managers/resource-manager.hpp"
#include "window.hpp"
#include <iostream>
#include <iterator>
#include <sys/mman.h>

namespace astralix {
RenderSystem::RenderSystem() {};

void RenderSystem::start() {
  Engine::get()->renderer_api->init();

  auto entity_manager = EntityManager::get();
  auto resource_manager = ResourceManager::get();

  resource_manager->load_shader(Shader::create("shaders::post_processing",
                                               "fragment/postprocessing.glsl",
                                               "vertex/postprocessing.glsl"));

  entity_manager->add_entity<PostProcessing>("Post Processing",
                                             "shaders::post_processing");

  entity_manager->for_each<Object>([](Object *object) { object->start(); });
  entity_manager->for_each<PostProcessing>(
      [](PostProcessing *post_processing) { post_processing->start(); });
}

void RenderSystem::fixed_update(double fixed_dt) {

};

void RenderSystem::pre_update(double dt) {
  auto engine = Engine::get();

  engine->framebuffer->bind();

  engine->renderer_api->enable_buffer_testing();
  engine->renderer_api->clear_color();
  engine->renderer_api->clear_buffers();
  engine->framebuffer->clear_attachment(1, -1);

  EntityManager::get()->for_each<Object>(
      [&](Object *object) { object->pre_update(); });
};

void RenderSystem::update(double dt) {
  auto entity_manager = EntityManager::get();

  entity_manager->for_each<Object>([&](Object *object) { object->update(); });

  auto scheduler = EventScheduler::get();

  scheduler->bind(SchedulerType::REALTIME);

  Engine::get()->framebuffer->unbind();

  entity_manager->for_each<Object>(
      [&](Object *object) { object->post_update(); });

  entity_manager->for_each<PostProcessing>(
      [&](PostProcessing *post_processing) { post_processing->post_update(); });
};

RenderSystem::~RenderSystem() {}

} // namespace astralix
