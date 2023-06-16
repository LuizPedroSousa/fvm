#include "post-processing.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/resource/resource-component.hpp"
#include "glad/glad.h"
#include "window.hpp"

namespace astralix {

PostProcessing::PostProcessing(ENTITY_INIT_PARAMS, ResourceID shader_id)
    : ENTITY_INIT() {
  add_component<MeshComponent>()->attach_mesh(Mesh::quad(1.0f));
  add_component<ResourceComponent>()->attach_shader(shader_id);
}

void PostProcessing::start() {
  if (is_active()) {

    create_framebuffer();

    auto resource = get_component<ResourceComponent>();
    auto mesh     = get_component<MeshComponent>();

    resource->start();
    mesh->start();
  }
}

void PostProcessing::post_update() {
  if (is_active()) {
    auto resource = get_component<ResourceComponent>();
    auto mesh     = get_component<MeshComponent>();

    resource->update();
    resource->get_shader_renderer_uniform()->setInt("screen_texture", 0);
    glBindTexture(GL_TEXTURE_2D, m_screen_texture);

    mesh->update();
  }
}

void PostProcessing::create_framebuffer() {

  glGenFramebuffers(1, &m_framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

  Window *window = Window::get();

  glGenTextures(1, &m_screen_texture);
  glBindTexture(GL_TEXTURE_2D, m_screen_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window->get_width(),
               window->get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         m_screen_texture, 0);

  glGenRenderbuffers(1, &m_renderbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                        window->get_width(), window->get_height());
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, m_renderbuffer);

  ASSERT_THROW(glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
                   GL_FRAMEBUFFER_COMPLETE,
               "Can't create framebuffer");
}

PostProcessing::~PostProcessing() {
  glDeleteFramebuffers(1, &m_framebuffer);
  glDeleteTextures(1, &m_screen_texture);
  glDeleteRenderbuffers(1, &m_renderbuffer);
}

} // namespace astralix
