
#pragma once

class Renderer
{
public:
  virtual ~Renderer() = default;
  virtual void render_all() = 0;
  virtual void start_all() = 0;
};