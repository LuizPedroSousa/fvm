#include "render-system.hpp"
#include "components/render/render-component.hpp"
#include "components/resource/resource-component.hpp"
#include "glad/glad.h"

#include "components/camera/camera-component.hpp"
#include "components/light/light-component.hpp"
#include "engine.hpp"
#include "entities/object.hpp"
#include "iostream"
#include "scene-system.hpp"
#include "window.hpp"

#include "entities/post-processing.hpp"

namespace astralix {

static unsigned int fbo = 0;
static unsigned int texture_color = 0;

RenderSystem::RenderSystem()
    : m_clear_color(glm::vec4(0.5f, 0.5f, 1.0f, 0.0f)){};

void RenderSystem::start() {
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  Window *window = Window::get();

  glGenTextures(1, &texture_color);
  glBindTexture(GL_TEXTURE_2D, texture_color);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window->get_width(),
               window->get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texture_color, 0);

  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);

  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                        window->get_width(), window->get_height());
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, rbo);

  ASSERT_THROW(glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
                   GL_FRAMEBUFFER_COMPLETE,
               "Can't create framebuffer");

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  auto manager = Engine::get()->get_entity_manager();

  auto post_processing = manager->add_entity<PostProcessing>();

  auto resource_manager = Engine::get()->get_resource_manager();
  resource_manager->load_shader("shaders::post_processing",
                                "vertex/postprocessing.glsl",
                                "fragment/postprocessing.glsl");

  post_processing.get_component<ResourceComponent>()->attach_shader(
      "shaders::post_processing");

  post_processing.get_component<RenderComponent>()->start(
      manager->get_entity(post_processing.get_entity_id()));

  manager->for_each<Object>([](Object *object) {
    if (object->is_active()) {
      auto render = object->get_component<RenderComponent>();

      if (render != nullptr && render->is_active()) {
        render->start(object);
      }
    }
  });
}

void RenderSystem::fixed_update(double fixed_dt){

};

void RenderSystem::pre_update(double dt) {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);
  glClearColor(m_clear_color.x, m_clear_color.y, m_clear_color.z,
               m_clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  auto manager = Engine::get()->get_entity_manager();
  manager->for_each<Object>([](Object *object) {
    auto resource = object->get_component<ResourceComponent>();
    auto render = object->get_component<RenderComponent>();

    if (render != nullptr && render->is_active()) {
      render->pre_update(object);
    }
  });
};

void RenderSystem::update(double dt) {
  auto manager = Engine::get()->get_entity_manager();

  manager->for_each<Object>([&](Object *object) {
    auto resource = object->get_component<ResourceComponent>();

    resource->get_shader_renderer()->use();

    auto render = object->get_component<RenderComponent>();

    if (render != nullptr && render->is_active()) {
      render->update(object);
    }
  });
};

void RenderSystem::post_update(double dt) {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDisable(GL_DEPTH_TEST);
  glClearColor(m_clear_color.x, m_clear_color.y, m_clear_color.z,
               m_clear_color.w);

  auto entity_manager = Engine::get()->get_entity_manager();

  entity_manager->for_each<PostProcessing>([](PostProcessing *entity) {
    auto resource = entity->get_component<ResourceComponent>();

    resource->get_shader_renderer()->use();

    auto render = entity->get_component<RenderComponent>();

    if (render != nullptr && render->is_active()) {
      resource->get_shader_renderer()->use();
      resource->get_shader_renderer_uniform()->setInt("screen_texture", 0);
      glBindTexture(GL_TEXTURE_2D, texture_color);
      render->update(entity);
    }
  });
};

RenderSystem::~RenderSystem() { glDeleteFramebuffers(1, &fbo); }

} // namespace astralix