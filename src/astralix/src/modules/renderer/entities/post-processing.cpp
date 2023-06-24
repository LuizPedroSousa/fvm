#include "post-processing.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/resource/resource-component.hpp"
#include "engine.hpp"
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

    bool has_msaa =
        Engine::get()->get_render_configuration()->has_msaa_enabled();

    if (has_msaa) {
      create_framebuffer(false, FrameBufferType::intermediate);
    }

    create_framebuffer(has_msaa, FrameBufferType::normal);

    auto resource = get_component<ResourceComponent>();
    auto mesh = get_component<MeshComponent>();

    resource->start();
    mesh->start();
  }
}

void PostProcessing::post_update(glm::vec4 clear_color) {
  if (is_active()) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    auto resource = get_component<ResourceComponent>();
    auto mesh = get_component<MeshComponent>();

    resource->update();

    bool is_msaa_enabled =
        Engine::get()->get_render_configuration()->has_msaa_enabled();

    if (is_msaa_enabled) {
      resolve_screen_texture();
    }

    resource->get_shader_renderer_uniform()->setInt("screen_texture", 0);

    unsigned int screen_texture =
        is_msaa_enabled ? m_multisampled_framebuffer->screen_texture
                        : m_framebuffer->screen_texture;

    glBindTexture(GL_TEXTURE_2D, screen_texture);

    mesh->update();
  }
}

void PostProcessing::resolve_screen_texture() {
  Window *window = Window::get();
  int width = window->get_width();
  int height = window->get_height();

  glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebuffer->fbo);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_multisampled_framebuffer->fbo);
  glBlitFramebuffer(0, 0, width, height, 0, 0, width, height,
                    GL_COLOR_BUFFER_BIT, GL_NEAREST);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessing::create_framebuffer(bool is_multisampled,
                                        FrameBufferType type) {

  FrameBuffer *framebuffer = new FrameBuffer;

  glGenFramebuffers(1, &framebuffer->fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->fbo);
  create_screen_texture(is_multisampled, *framebuffer);
  create_render_buffer(is_multisampled, *framebuffer);

  ASSERT_THROW(glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
                   GL_FRAMEBUFFER_COMPLETE,
               "Can't create framebuffer");

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  if (type == FrameBufferType::normal) {
    m_framebuffer = framebuffer;
  } else {
    m_multisampled_framebuffer = framebuffer;
  }
};

void PostProcessing::create_render_buffer(bool is_multisampled,
                                          FrameBuffer &framebuffer) {
  glGenRenderbuffers(1, &framebuffer.rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, framebuffer.rbo);

  RenderConfiguration *render_configuration =
      Engine::get()->get_render_configuration();

  Window *window = Window::get();

  if (is_multisampled) {
    glRenderbufferStorageMultisample(
        GL_RENDERBUFFER, render_configuration->msaa.samples,
        GL_DEPTH24_STENCIL8, window->get_width(), window->get_height());
  } else {
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                          window->get_width(), window->get_height());
  }

  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, framebuffer.rbo);
};

void PostProcessing::create_screen_texture(bool is_multisampled,
                                           FrameBuffer &framebuffer) {
  glGenTextures(1, &framebuffer.screen_texture);

  Window *window = Window::get();

  RenderConfiguration *render_configuration =
      Engine::get()->get_render_configuration();

  if (is_multisampled) {
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebuffer.screen_texture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,
                            render_configuration->msaa.samples, GL_RGB,
                            window->get_width(), window->get_height(), GL_TRUE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D_MULTISAMPLE,
                           framebuffer.screen_texture, 0);

  } else {
    glBindTexture(GL_TEXTURE_2D, framebuffer.screen_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window->get_width(),
                 window->get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           framebuffer.screen_texture, 0);
  }
};

PostProcessing::~PostProcessing() {
  glDeleteFramebuffers(1, &m_framebuffer->fbo);
  glDeleteTextures(1, &m_framebuffer->fbo);
  glDeleteRenderbuffers(1, &m_framebuffer->fbo);

  delete m_framebuffer;
  m_framebuffer = nullptr;

  if (m_multisampled_framebuffer != nullptr) {
    glDeleteFramebuffers(1, &m_multisampled_framebuffer->fbo);
    glDeleteTextures(1, &m_multisampled_framebuffer->fbo);
    glDeleteRenderbuffers(1, &m_multisampled_framebuffer->fbo);

    delete m_multisampled_framebuffer;
    m_multisampled_framebuffer = nullptr;
  }
}
} // namespace astralix
