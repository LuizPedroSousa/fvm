#include "light-component.hpp"
#include "application.hpp"
#include "components/resource/resource-component.hpp"
#include "components/transform/transform-component.hpp"
#include "engine.hpp"
#include "entities/light-source.hpp"
#include "iostream"
#include "memory"
#include "resources/shader.hpp"

namespace astralix {

LightComponent::LightComponent(COMPONENT_INIT_PARAMS,
                               std::unique_ptr<LightStrategy> strategy,
                               CameraComponent *camera)
    : COMPONENT_INIT(LightComponent), m_camera(camera),
      m_strategy(std::move(strategy)){};

void LightComponent::add_source(glm::vec3 position, ResourceID shader_id) {
  auto manager = Engine::get()->get_entity_manager();

  auto object = manager->add_entity<LightSource>(position);

  auto resource = object.get_component<ResourceComponent>();

  resource->attach_shader(shader_id);

  m_source = manager->get_entity<LightSource>(object.get_entity_id());
};

void LightComponent::start() {
  auto transform = m_source->get_component<TransformComponent>();

  transform->scale_initial(glm::vec3(1.0f));
}

void LightComponent::update(Object *object) {
  auto resource = object->get_component<ResourceComponent>();

  if (resource != nullptr) {
    auto transform = m_source->get_component<TransformComponent>();

    resource->get_shader_renderer_uniform()->setVec3("view_position",
                                                     m_camera->get_position());
  }

  m_strategy->update(m_source, object, m_camera);
}

} // namespace astralix
