#pragma once

#include "ecs/entities/entity.hpp"

namespace astralix {
class PostProcessing : public Entity<PostProcessing> {
public:
  PostProcessing(ENTITY_INIT_PARAMS, ResourceID shader_id);
  ~PostProcessing();

  void start();
  void post_update();

  void on_enable() override{};
  void on_disable() override{};

  unsigned int get_framebuffer() { return m_framebuffer; }

private:
  void create_framebuffer();

  unsigned int m_framebuffer    = 0;
  unsigned int m_screen_texture = 0;
  unsigned int m_renderbuffer   = 0;
};
} // namespace astralix