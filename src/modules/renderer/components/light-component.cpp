#include "light-component.hpp"
#include "application.hpp"
#include "components/resource-component.hpp"
#include "components/transform-component.hpp"
#include "game.hpp"
#include "iostream"
#include "memory"
#include "resources/shader.hpp"

LightComponent::LightComponent(COMPONENT_INIT_PARAMS, CameraComponent *camera) : COMPONENT_INIT(LightComponent) {
  auto owner = Game::get()->get_entity_manager()->get_entity(get_owner_id());
  auto owner_resource = owner->get_component<ResourceComponent>();

  auto owner_shader_loaded = owner_resource->load_shader(Shader::create("vertex/light_source.glsl", "fragment/light_source.glsl"));

  ASSERT_COMPARE_THROW(owner_shader_loaded);

  m_camera = camera;
};

void LightComponent::start(Uniform *uniform) {
}

void LightComponent::update(Uniform *uniform) {
  auto entities = Game::get()->get_entity_manager()->get_entities();
  auto owner = Game::get()->get_entity_manager()->get_entity(get_owner_id());

  for (auto entity = entities.begin(); entity != entities.end(); ++entity) {
    if (entity->first == get_owner_id()) continue;
    auto resource = entity->second->get_component<ResourceComponent>();
    auto transform = owner->get_component<TransformComponent>();

    transform->scale(glm::vec3(0.2f));

    if (resource != nullptr) {
      entity->second->get_component<TransformComponent>()->scale(glm::vec3(2.0f, 0.2f, 0.2f));

      resource->get_shader_renderer()->use();
      resource->get_shader_renderer()->get_uniform()->setVec3("light_position", *(transform->get_position()));
      resource->get_shader_renderer()->get_uniform()->setVec3("view_position", m_camera->get_position());

      resource->get_shader_renderer()->get_uniform()->setVec3("light.ambient", glm::vec3(0.5f));
      resource->get_shader_renderer()->get_uniform()->setVec3("light.diffuse", glm::vec3(0.4f));
      resource->get_shader_renderer()->get_uniform()->setVec3("light.specular", glm::vec3(1.0f));
    }
  }
}