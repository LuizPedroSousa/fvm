#include "light-component.hpp"
#include "application.hpp"
#include "game.hpp"
#include "iostream"
#include "memory"
#include "model-component.hpp"
#include "resource-component.hpp"
#include "resources/shader.hpp"
#include "transform-component.hpp"

LightComponent::LightComponent(COMPONENT_INIT_PARAMS, CameraComponent *camera)
    : COMPONENT_INIT(LightComponent) {

  m_camera = camera;
};

void LightComponent::add_source(glm::vec3 position, ResourceID shader_id) {
  auto manager = Game::get()->get_entity_manager();

  auto object = manager->add_entity<Object>(position);

  auto resource = object.get_component<ResourceComponent>();

  resource->attach_shader(shader_id);

  m_source = static_cast<Object *>(manager->get_entity(object.get_entity_id()));

  m_light = {.type = LightType::Point};
};

void LightComponent::start() {
  auto transform = m_source->get_component<TransformComponent>();

  transform->scale_initial(glm::vec3(1.0f));
}

void LightComponent::use_directional() {
  m_light = {.type = LightType::Directional};
}

void LightComponent::use_spotlight() {
  m_light = {.type = LightType::Spotlight};
}

void LightComponent::use_point() { m_light = {.type = LightType::Point}; }

void LightComponent::update(Object *object) {
  auto resource = object->get_component<ResourceComponent>();

  if (resource != nullptr) {
    auto transform = m_source->get_component<TransformComponent>();

    resource->get_shader_renderer_uniform()->setVec3("view_position",
                                                     m_camera->get_position());

    switch (m_light.type) {
    case LightType::Directional:

      resource->get_shader_renderer_uniform()->setVec3(
          "directional_light.direction",
          transform->get_render_transform()->position);
      resource->get_shader_renderer_uniform()->setVec3(
          "directional_light.exposure.ambient", glm::vec3(0.2f));
      resource->get_shader_renderer_uniform()->setVec3(
          "directional_light.exposure.diffuse", glm::vec3(0.5f));
      resource->get_shader_renderer_uniform()->setVec3(
          "directional_light.exposure.specular", glm::vec3(1.0f));

      break;

    case LightType::Spotlight:
      resource->get_shader_renderer_uniform()->setVec3(
          "spot_light.position", m_camera->get_position());
      resource->get_shader_renderer_uniform()->setVec3("spot_light.direction",
                                                       m_camera->get_front());
      resource->get_shader_renderer_uniform()->setFloat(
          "spot_light.inner_cut_off", glm::cos(glm::radians(12.5f)));
      resource->get_shader_renderer_uniform()->setFloat(
          "spot_light.outer_cut_off", glm::cos(glm::radians(20.5f)));
      resource->get_shader_renderer_uniform()->setVec3(
          "spot_light.exposure.ambient", glm::vec3(0.2f));
      resource->get_shader_renderer_uniform()->setVec3(
          "spot_light.exposure.diffuse", glm::vec3(0.5f));
      resource->get_shader_renderer_uniform()->setVec3(
          "spot_light.exposure.specular", glm::vec3(1.0f));
      break;

    default:
      resource->get_shader_renderer_uniform()->setFloat(
          "point_lights[0].attenuation.constant", 1.0f);
      resource->get_shader_renderer_uniform()->setFloat(
          "point_lights[0].attenuation.linear", 0.09f);
      resource->get_shader_renderer_uniform()->setFloat(
          "point_lights[0].attenuation.quadratic", 0.032f);
      resource->get_shader_renderer_uniform()->setVec3(
          "point_lights[0].exposure.ambient", glm::vec3(1.0f));
      resource->get_shader_renderer_uniform()->setVec3(
          "point_lights[0].exposure.diffuse", glm::vec3(0.5f));
      resource->get_shader_renderer_uniform()->setVec3(
          "point_lights[0].exposure.specular", glm::vec3(1.0f));
      resource->get_shader_renderer_uniform()->setVec3(
          "point_lights[0].position",
          transform->get_render_transform()->position);
      break;
    }
  }
}