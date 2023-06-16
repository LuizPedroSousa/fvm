#include "skybox.hpp"
#include "components/camera/camera-component.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/resource/resource-component.hpp"
#include "engine.hpp"
#include "glad/glad.h"

namespace astralix {
Skybox::Skybox(ENTITY_INIT_PARAMS, const ResourceID cubemap_id,
               const ResourceID shader_id)
    : ENTITY_INIT() {
  auto mesh     = add_component<MeshComponent>();
  auto resource = add_component<ResourceComponent>();

  resource->attach_cubemap(cubemap_id);
  resource->attach_shader(shader_id);

  mesh->attach_mesh(Mesh::cube_mesh(2.0f));
};

void Skybox::start() {
  get_component<ResourceComponent>()->start();
  get_component<MeshComponent>()->start();
}

void Skybox::update() {
  glDepthFunc(GL_LEQUAL);

  auto resource = get_component<ResourceComponent>();
  auto mesh     = get_component<MeshComponent>();
  auto camera =
      Engine::get()->get_component_manager()->get_component<CameraComponent>();

  resource->update();

  if (camera != nullptr) {
    auto uniform = resource->get_shader_renderer_uniform();
    auto view_without_transformation =
        glm::mat4(glm::mat3(camera->get_view_matrix()));
    uniform->setMatrix("view", view_without_transformation);
    uniform->setMatrix("projection", camera->get_projection_matrix());
  }

  mesh->update();

  glDepthFunc(GL_LESS);
}

} // namespace astralix
