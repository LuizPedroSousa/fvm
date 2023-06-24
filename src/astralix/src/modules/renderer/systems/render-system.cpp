#include "render-system.hpp"
#include "glad/glad.h"

#include "entities/skybox.hpp"

#include "engine.hpp"
#include "entities/object.hpp"
#include "iostream"
#include "scene-system.hpp"
#include "window.hpp"

#include "entities/post-processing.hpp"

namespace astralix {

RenderSystem::RenderSystem()
    : m_clear_color(glm::vec4(0.5f, 0.5f, 1.0f, 0.0f)){};

void RenderSystem::start() {
  glEnable(GL_MULTISAMPLE);

  auto manager = Engine::get()->get_entity_manager();

  auto post_processing = manager->get_entity<PostProcessing>();

  if (post_processing != nullptr) {
    post_processing->start();
  }

  manager->for_each<Object>([](Object *object) { object->start(); });

  auto skybox = manager->get_entity<Skybox>();

  if (skybox != nullptr) {
    skybox->start();
  }
}

void RenderSystem::fixed_update(double fixed_dt){

};

void RenderSystem::pre_update(double dt) {
  auto manager = Engine::get()->get_entity_manager();
  auto post_processing = manager->get_entity<PostProcessing>();
  bool has_post_processing =
      post_processing != nullptr && post_processing->is_active();
  int default_framebuffer = 0;

  glBindFramebuffer(GL_FRAMEBUFFER, has_post_processing
                                        ? post_processing->get_framebuffer().fbo
                                        : default_framebuffer);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);
  glClearColor(m_clear_color.x, m_clear_color.y, m_clear_color.z,
               m_clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
};

void RenderSystem::update(double dt) {
  auto manager = Engine::get()->get_entity_manager();
  auto skybox = manager->get_entity<Skybox>();

  manager->for_each<Object>([&](Object *object) { object->update(); });

  if (skybox != nullptr) {
    skybox->update();
  }
};

void RenderSystem::post_update(double dt) {
  auto entity_manager = Engine::get()->get_entity_manager();
  auto post_processing = entity_manager->get_entity<PostProcessing>();

  if (post_processing != nullptr) {
    post_processing->post_update(m_clear_color);
  }
};

RenderSystem::~RenderSystem() {}

} // namespace astralix