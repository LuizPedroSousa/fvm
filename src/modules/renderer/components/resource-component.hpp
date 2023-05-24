#pragma once
#include "components/base/component.hpp"
#include "resources/shader-renderer.hpp"
#include "resources/shader.hpp"
#include "resources/texture-renderer.hpp"
#include "resources/texture.hpp"

class ResourceComponent : public Component<ResourceComponent> {
  public:
  ResourceComponent(COMPONENT_INIT_PARAMS);
  ResourceComponent() {}

  Either<BaseException, Unit> load_texture(Either<BaseException, Texture> texture);
  Either<BaseException, Unit> load_shader(Either<BaseException, Shader> shader);

  void start();
  void update();

  unsigned int get_shader_renderer_id() {
    return m_shader_renderer.id;
  }

  ShaderRenderer *get_shader_renderer() {
    return &m_shader_renderer;
  }

  Uniform *get_shader_renderer_uniform() {
    return m_shader_renderer.get_uniform();
  }

  private:
  TextureRenderer m_texture_renderer;
  ShaderRenderer m_shader_renderer;
};