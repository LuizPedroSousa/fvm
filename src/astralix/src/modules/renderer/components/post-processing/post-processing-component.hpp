#pragma once

#include "base.hpp"
#include "ecs/components/component.hpp"
#include "framebuffer.hpp"
#include "glm/glm.hpp"

namespace astralix {

  class PostProcessingComponent
    : public Component<PostProcessingComponent>
  {
  public:
    PostProcessingComponent(COMPONENT_INIT_PARAMS);
    ~PostProcessingComponent() = default;

    void start();
    void post_update();

  private:
    void resolve_screen_texture();

    Ref<Framebuffer> m_multisampled_framebuffer;
  };
} // namespace astralix