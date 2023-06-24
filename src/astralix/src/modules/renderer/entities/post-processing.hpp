#pragma once

#include "ecs/entities/entity.hpp"
#include "glm/glm.hpp"

namespace astralix {

struct FrameBuffer {
  unsigned int fbo = 0;
  unsigned int rbo = 0;
  unsigned int screen_texture = 0;
};

enum FrameBufferType { normal = 0, intermediate = 1 };

class PostProcessing : public Entity<PostProcessing> {
public:
  PostProcessing(ENTITY_INIT_PARAMS, ResourceID shader_id);
  ~PostProcessing();

  void start();
  void post_update(glm::vec4 clear_color);

  void on_enable() override{};
  void on_disable() override{};

  FrameBuffer &get_framebuffer() { return *m_framebuffer; }

private:
  void create_framebuffer(bool is_multisampled, FrameBufferType type);
  void create_render_buffer(bool is_multisampled, FrameBuffer &framebuffer);
  void create_screen_texture(bool is_multisampled, FrameBuffer &framebuffer);
  void resolve_screen_texture();

  FrameBuffer *m_framebuffer;
  FrameBuffer *m_multisampled_framebuffer;
};
} // namespace astralix