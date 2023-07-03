#include "post-processing-component.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/resource/resource-component.hpp"
#include "engine.hpp"
#include "glad/glad.h"
#include "window.hpp"

namespace astralix {

PostProcessingComponent::PostProcessingComponent(COMPONENT_INIT_PARAMS)
    : COMPONENT_INIT(PostProcessingComponent, "Post Processing", true) {}

void PostProcessingComponent::start() {
  auto owner = get_owner();
  auto mesh = owner->get_or_add_component<MeshComponent>();
  auto resource = owner->get_component<ResourceComponent>();

  if (Engine::get()->has_msaa_enabled()) {
    auto framebuffer = Engine::get()->framebuffer;

    FramebufferSpecification framebuffer_spec =
        framebuffer->get_specification();

    framebuffer_spec.samples = Engine::get()->msaa.samples;

    m_multisampled_framebuffer = Framebuffer::create(framebuffer_spec);
  }
}

void PostProcessingComponent::post_update() {
  auto owner = get_owner();
  auto resource = owner->get_component<ResourceComponent>();

  bool is_msaa_enabled = Engine::get()->has_msaa_enabled();

  if (is_msaa_enabled) {
    resolve_screen_texture();
  }

  resource->get_shader()->set_int("screen_texture", 0);

  u_int32_t screen_texture =
      is_msaa_enabled ? m_multisampled_framebuffer->get_color_attachment_id()
                      : Engine::get()->framebuffer->get_color_attachment_id();

  glBindTexture(GL_TEXTURE_2D, screen_texture);
}

void PostProcessingComponent::resolve_screen_texture() {
  Window *window = Window::get();
  int width = window->get_width();
  int height = window->get_height();

  Ref<Framebuffer> framebuffer = Engine::get()->framebuffer;

  framebuffer->bind(FramebufferBindType::Read);
  m_multisampled_framebuffer->bind(FramebufferBindType::Draw);

  const FramebufferSpecification &spec = framebuffer->get_specification();
  framebuffer->blit(spec.width, spec.height);

  framebuffer->unbind();
}

} // namespace astralix
