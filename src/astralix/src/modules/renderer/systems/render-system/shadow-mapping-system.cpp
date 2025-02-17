#include "systems/render-system/shadow-mapping-system.hpp"
#include "components/resource/resource-component.hpp"
#include "ecs/managers/entity-manager.hpp"
#include "engine.hpp"
#include "entities/object.hpp"
#include "managers/resource-manager.hpp"

#include "glad/glad.h"

namespace astralix {

ShadowMappingSystem::ShadowMappingSystem() {}
ShadowMappingSystem::~ShadowMappingSystem() {}

void ShadowMappingSystem::start() {
  glEnable(GL_DEPTH_TEST);

  auto resource_manager = ResourceManager::get();

  FramebufferSpecification framebuffer_spec;

  framebuffer_spec.attachments = {FramebufferTextureFormat::DEPTH_ONLY};

  framebuffer_spec.width = 1920;
  framebuffer_spec.height = 1080;

  m_framebuffer = std::move(Framebuffer::create(framebuffer_spec));

  auto shader = resource_manager->load_shader(Shader::create(
      "shadow_mapping_depth", "fragment/shadow_mapping_depth.glsl",
      "vertex/shadow_mapping_depth.glsl"));

  shader->attach();
}

void ShadowMappingSystem::bind_depth() {
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_framebuffer->get_color_attachment_id());
}

void ShadowMappingSystem::pre_update(double dt) {}

void ShadowMappingSystem::fixed_update(double fixed_dt) {}

void ShadowMappingSystem::update(double dt) {
  auto engine = Engine::get();

  auto entity_manager = EntityManager::get();

  m_framebuffer->bind();

  engine->renderer_api->enable_buffer_testing();
  engine->renderer_api->clear_buffers();
  engine->renderer_api->clear_color();

  glCullFace(GL_FRONT);

  entity_manager->for_each<Object>([&](Object *object) {
    auto resource = object->get_component<ResourceComponent>();

    if (resource != nullptr) {
      if (resource->has_shader()) {
        resource->set_shader("shadow_mapping_depth");
      }
    }

    object->update();
  });

  glCullFace(GL_BACK);

  m_framebuffer->unbind();
}

} // namespace astralix
