#include "camera.hpp"
#include "object-renderer.hpp"
#include "scene.hpp"
#include "shader-renderer.hpp"
#include "texture-renderer.hpp"

class Prologue : public Scene {

  public:
  void start() override;
  void update() override;

  private:
  ShaderRenderer m_shader_renderer;
  ObjectRenderer m_object_renderer;
  TextureRenderer m_texture_renderer;
  Camera m_camera;
};