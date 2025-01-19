#pragma once

#include "framebuffer.hpp"
#include "renderer-api.hpp"

namespace astralix {

class Engine {
public:
  static void init();
  void end();

  static Engine *get() { return m_instance; }

  void start();
  void update();

  struct MSAA {
    int samples;
    bool is_enabled;
  };

  bool has_msaa_enabled() { return msaa.is_enabled; }

  Ref<Framebuffer> framebuffer;
  Scope<RendererAPI> renderer_api;
  MSAA msaa;

private:
  Engine();
  ~Engine() = default;

  static Engine *m_instance;
};

}; // namespace astralix
