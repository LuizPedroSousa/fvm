#include "render-system.hpp"
#include "components/camera/camera-component.hpp"
#include "components/light/light-component.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/post-processing/post-processing-component.hpp"
#include "components/resource/resource-component.hpp"
#include "components/transform/transform-component.hpp"
#include "ecs/entities/entity.hpp"
#include "ecs/entities/ientity.hpp"
#include "ecs/managers/entity-manager.hpp"
#include "entities/camera.hpp"
#include "events/event-dispatcher.hpp"
#include "events/event-scheduler.hpp"
#include "events/key-codes.hpp"
#include "events/key-event.hpp"
#include "framebuffer.hpp"
#include "glad/glad.h"

#include "debug-system.hpp"
#include "engine.hpp"
#include "entities/object.hpp"
#include "entities/post-processing.hpp"
#include "entities/skybox.hpp"
#include "entities/text.hpp"
#include "iostream"
#include "log.hpp"
#include "managers/resource-manager.hpp"
#include "renderer-api.hpp"
#include "resources/shader.hpp"
#include "systems/render-system/shadow-mapping-system.hpp"
#include "window.hpp"
#include <iostream>
#include <iterator>
#include <sys/mman.h>

namespace astralix {
RenderSystem::RenderSystem() {};

void RenderSystem::start() {
  Engine::get()->renderer_api->init();

  auto entity_manager = EntityManager::get();

  EntityManager::get()->for_each<Skybox>(
      [&](Skybox *skybox) { skybox->start(); });
  entity_manager->for_each<Object>([](Object *object) { object->start(); });
  entity_manager->for_each<PostProcessing>(
      [](PostProcessing *post_processing) { post_processing->start(); });

  entity_manager->for_each<Text>([&](Text *text) { text->start(); });

  add_subsystem<ShadowMappingSystem>()->start();
  add_subsystem<DebugSystem>()->start();
}

void RenderSystem::fixed_update(double fixed_dt) {
  auto entity_manager = EntityManager::get();

  entity_manager->for_each<Object>(
      [&](Object *object) { object->fixed_update(fixed_dt); });
};

void RenderSystem::pre_update(double dt) {
  auto engine = Engine::get();

  auto entity_manager = EntityManager::get();

  auto post_processings = entity_manager->get_entities<PostProcessing>();

  bool has_post_processing = false;

  for (auto post_processing : post_processings) {
    if (!post_processing->is_active()) {
      continue;
    }

    auto comp = post_processing->get_component<PostProcessingComponent>();
    if (comp != nullptr && comp->is_active()) {
      has_post_processing = true;
      break;
    }
  }

  if (has_post_processing) {
    engine->framebuffer->bind();
  } else {
    engine->framebuffer->bind(FramebufferBindType::Default, 0);
  }

  engine->renderer_api->enable_buffer_testing();
  engine->renderer_api->clear_color();
  engine->renderer_api->clear_buffers();
  engine->framebuffer->clear_attachment(1, -1);

  EntityManager::get()->for_each<Object>(
      [&](Object *object) { object->pre_update(); });

  EntityManager::get()->for_each<Skybox>(
      [&](Skybox *skybox) { skybox->pre_update(); });
};

void RenderSystem::update(double dt) {
  auto entity_manager = EntityManager::get();

  auto shadow_mapping = get_subsystem<ShadowMappingSystem>();
  auto debug = get_subsystem<DebugSystem>();

  if (shadow_mapping != nullptr) {
    shadow_mapping->update(dt);
  }

  EntityManager::get()->for_each<Skybox>(
      [&](Skybox *skybox) { skybox->update(); });

  entity_manager->for_each<Camera>([&](Camera *object) { object->update(); });

  entity_manager->for_each<Object>([&](Object *object) {
    if (shadow_mapping != nullptr) {
      shadow_mapping->bind_depth(object);
    }

    object->update();
  });

  entity_manager->for_each<Text>([&](Text *text) { text->update(); });

  auto scheduler = EventScheduler::get();

  if (debug != nullptr)
    debug->update(dt);

  scheduler->bind(SchedulerType::REALTIME);

  Engine::get()->framebuffer->unbind();

  entity_manager->for_each<Skybox>(
      [&](Skybox *skybox) { skybox->post_update(); });

  entity_manager->for_each<Object>(
      [&](Object *object) { object->post_update(); });

  entity_manager->for_each<PostProcessing>(
      [&](PostProcessing *post_processing) { post_processing->post_update(); });
};

RenderSystem::~RenderSystem() {}

} // namespace astralix
