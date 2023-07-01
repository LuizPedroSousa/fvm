#pragma once

#include "entities/layer.hpp"
#include "glm/glm.hpp"

namespace astralix {

class ViewportLayer : public Layer {
public:
  ViewportLayer() : Layer("Viewport"){};

  void start() override;
  void update() override;

private:
  bool m_viewport_focused = false, m_viewport_hovered = false;
  glm::vec2 m_viewport_size = {0.0f, 0.0f};
  glm::vec2 m_viewport_bounds[2];
};

} // namespace astralix