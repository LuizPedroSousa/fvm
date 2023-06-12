#pragma once

namespace astralix {

class Renderer {
public:
  virtual ~Renderer() = default;
  virtual void render_all() = 0;
  virtual void start_all() = 0;
};

} // namespace astralix