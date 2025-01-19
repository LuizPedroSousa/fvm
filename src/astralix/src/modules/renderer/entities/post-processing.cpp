#include "post-processing.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/post-processing/post-processing-component.hpp"
#include "components/resource/resource-component.hpp"
#include "engine.hpp"
#include "glad/glad.h"
#include "window.hpp"

namespace astralix {

PostProcessing::PostProcessing(ENTITY_INIT_PARAMS, ResourceID shader_id)
    : ENTITY_INIT() {
  add_component<MeshComponent>()->attach_mesh(Mesh::quad(1.0f));
  add_component<PostProcessingComponent>();
  add_component<ResourceComponent>()->set_shader(shader_id);
}

void PostProcessing::start() {
  if (is_active()) {
    auto resource = get_component<ResourceComponent>();
    auto mesh = get_component<MeshComponent>();
    auto post_processing = get_component<PostProcessingComponent>();

    resource->start();
    post_processing->start();
    mesh->start();
  }
}

void PostProcessing::post_update() {
  if (is_active()) {
    Engine::get()->framebuffer->bind(FramebufferBindType::Default, 0);
    Engine::get()->renderer_api->disable_buffer_testing();
    Engine::get()->renderer_api->clear_color();
    Engine::get()->renderer_api->clear_buffers();

    auto resource = get_component<ResourceComponent>();
    auto post_processing = get_component<PostProcessingComponent>();
    auto mesh = get_component<MeshComponent>();

    resource->update();
    post_processing->post_update();
    mesh->update();
  }
}

} // namespace astralix
