#include "skybox-component.hpp"
#include "components/camera/camera-component.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/resource/resource-component.hpp"

#include "glad/glad.h"

namespace astralix {
SkyboxComponent::SkyboxComponent(COMPONENT_INIT_PARAMS,
                                 const ResourceID cubemap_id,
                                 const ResourceID skybox_shader_id)
    : COMPONENT_INIT(SkyboxComponent, "Skybox", true) {

  auto owner = get_owner();
  auto mesh = owner->get_or_add_component<MeshComponent>();
  auto resource = owner->get_component<ResourceComponent>();

  resource->attach_cubemap({cubemap_id, "_skybox"});
  resource->attach_shader(skybox_shader_id);

  mesh->attach_mesh(Mesh::cube(2.0f));
};

void SkyboxComponent::start() {}

void SkyboxComponent::pre_update() { glDepthFunc(GL_LEQUAL); }

void SkyboxComponent::update() {

  auto owner = get_owner();

  auto resource = owner->get_component<ResourceComponent>();

  auto entity_manager = EntityManager::get();

  if (!entity_manager->has_entity_with_component<CameraComponent>()) {
    return;
  }

  auto component_manager = ComponentManager::get();

  auto camera = component_manager->get_component<CameraComponent>();

  if (camera != nullptr) {
    auto renderer = resource->get_shader_renderer();
    renderer->use();

    auto uniform = renderer->get_uniform();

    auto view_without_transformation =
        glm::mat4(glm::mat3(camera->get_view_matrix()));
    uniform->setMatrix("view_without_transformation",
                       view_without_transformation);
    uniform->setMatrix("projection", camera->get_projection_matrix());
  }
}

void SkyboxComponent::post_update() { glDepthFunc(GL_LESS); }

} // namespace astralix