#include "systems/render-system/shadow-mapping-system.hpp"
#include "base.hpp"
#include "components/light/light-component.hpp"
#include "components/material/material-component.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/resource/resource-component.hpp"
#include "components/transform/transform-component.hpp"
#include "engine.hpp"
#include "entities/object.hpp"
#include "guid.hpp"
#include "log.hpp"
#include "managers/entity-manager.hpp"
#include "managers/resource-manager.hpp"
#include "path.hpp"
#include "systems/render-system/mesh-system.hpp"

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

  auto shader = resource_manager->load_shader(
      Shader::create("shadow_mapping_depth",
                     "shaders/fragment/shadow_mapping_depth.glsl"_engine,
                     "shaders/vertex/shadow_mapping_depth.glsl"_engine));

  shader->attach();
}

void ShadowMappingSystem::bind_depth(Object *object) {
  CHECK_ACTIVE(this);

  auto resource = object->get_component<ResourceComponent>();

  if (resource == nullptr || !resource->has_shader()) {
    return;
  }

  auto shader = resource->get_shader();

  auto manager = ResourceManager::get();

  int slot = manager->get_texture_slot() + 1;

  shader->set_int("shadowMap", slot);

  glActiveTexture(GL_TEXTURE0 + slot);

  glBindTexture(GL_TEXTURE_2D, m_framebuffer->get_color_attachment_id());
}

void ShadowMappingSystem::pre_update(double dt) {}

void ShadowMappingSystem::fixed_update(double fixed_dt) {}

void ShadowMappingSystem::update(double dt) {
  CHECK_ACTIVE(this);

  auto engine = Engine::get();

  auto entity_manager = EntityManager::get();

  m_framebuffer->bind();

  glEnable(GL_DEPTH_TEST);

  auto system_manager = SystemManager::get();

  auto mesh = system_manager->get_system<MeshSystem>();

  engine->renderer_api->enable_buffer_testing();
  engine->renderer_api->clear_buffers();
  engine->renderer_api->clear_color();

  auto component_manager = ComponentManager::get();
  auto light_components = component_manager->get_components<LightComponent>();

  entity_manager->for_each<Object>([&](Object *object) {
    glCullFace(GL_FRONT);
    auto resource = object->get_component<ResourceComponent>();

    auto mesh = object->get_component<MeshComponent>();
    auto transform = object->get_component<TransformComponent>();
    auto material = object->get_component<MaterialComponent>();

    if (resource == nullptr || !resource->has_shader()) {
      return;
    }

    ResourceID older_shader_id = resource->get_shader()->get_resource_id();

    resource->set_shader("shadow_mapping_depth");

    resource->update();

    if (transform != nullptr) {
      transform->update();
    }

    if (material != nullptr) {
      material->update();
    }

    for (size_t i = 0; i < light_components.size(); i++) {
      light_components[i]->update(object, i);
    }

    auto shader = resource->get_shader();

    shader->set_matrix("g_model", transform->matrix);

    if (mesh != nullptr) {
      mesh->update();
    }

    resource->set_shader(older_shader_id);

    glCullFace(GL_BACK);
  });

  m_framebuffer->unbind();

  engine->framebuffer->bind();
}

} // namespace astralix
