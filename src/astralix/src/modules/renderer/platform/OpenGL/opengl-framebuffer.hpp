#pragma once
#include "framebuffer.hpp"
#include "glad/glad.h"

namespace astralix {

class OpenGLFramebuffer : public Framebuffer {
public:
  OpenGLFramebuffer(const FramebufferSpecification &spec);
  ~OpenGLFramebuffer() override;

  void bind(FramebufferBindType type = FramebufferBindType::Default,
            u_int32_t id = -1) override;
  void unbind() override;

  void resize(uint32_t width, uint32_t height) override;
  int read_pixel(uint32_t attachment_index, int x, int y) override;

  void blit(uint32_t width, uint32_t height) override;

  void clear_attachment(uint32_t attachment_index, int value) override;

  uint32_t get_color_attachment_id(uint32_t index = 0) const override {
    return m_color_attachments[index];
  };

  const FramebufferSpecification &get_specification() const override {
    return m_specification;
  };

private:
  FramebufferSpecification m_specification;
  uint32_t m_renderer_id = 0;
  std::vector<FramebufferTextureSpecification>
      m_color_attachment_specifications;
  std::vector<uint32_t> m_color_attachments;

  FramebufferTextureSpecification m_depth_attachment_specification =
      FramebufferTextureFormat::None;
  uint32_t m_depth_attachment = 0;

  void invalidate();
};
} // namespace astralix
