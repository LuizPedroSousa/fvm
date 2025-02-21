#include "systems/render-system/shadow-mapping-system.hpp"
#include "base.hpp"
#include "components/resource/resource-component.hpp"
#include "ecs/guid.hpp"
#include "ecs/managers/entity-manager.hpp"
#include "engine.hpp"
#include "entities/object.hpp"
#include "managers/resource-manager.hpp"

#include "glad/glad.h"

namespace astralix {

ShadowMappingSystem::ShadowMappingSystem() {}
ShadowMappingSystem::~ShadowMappingSystem() {}

void ShadowMappingSystem::start() {

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

void ShadowMappingSystem::bind_depth(Object *object) {
  CHECK_ACTIVE(this);

  auto resource = object->get_component<ResourceComponent>();

  if (resource == nullptr || !resource->has_shader()) {
    return;
  }

  auto shader = resource->get_shader();

  shader->bind();

  shader->set_int("depthMap", 6);

  glActiveTexture(GL_TEXTURE6);
  glBindTexture(GL_TEXTURE_2D, m_framebuffer->get_color_attachment_id());

  shader->unbind();
}

void ShadowMappingSystem::pre_update(double dt) {}

void ShadowMappingSystem::fixed_update(double fixed_dt) {}

void ShadowMappingSystem::update(double dt) {
  CHECK_ACTIVE(this);

  auto engine = Engine::get();

  auto entity_manager = EntityManager::get();

  m_framebuffer->bind();

  glEnable(GL_DEPTH_TEST);

  engine->renderer_api->enable_buffer_testing();
  engine->renderer_api->clear_buffers();
  engine->renderer_api->clear_color();

  glCullFace(GL_FRONT);

  entity_manager->for_each<Object>([&](Object *object) {
    auto resource = object->get_component<ResourceComponent>();

    ResourceID older_shader_id;

    if (resource != nullptr && resource->has_shader()) {
      auto current_shader = resource->get_shader();
      older_shader_id = current_shader->get_resource_id();

      resource->set_shader("shadow_mapping_depth");
    }

    object->update();

    if (!older_shader_id.empty()) {
      resource->set_shader(older_shader_id);
    }
  });

  glCullFace(GL_BACK);

  m_framebuffer->unbind();
}

} // namespace astralix
