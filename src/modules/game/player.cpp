#include "player.hpp"
#include "glm/glm.hpp"

Player::Player(unsigned int render_id) {
  ObjectRenderer o_renderer;
  ShaderRenderer s_renderer;
  TextureRenderer t_renderer;

  auto shader_attached = s_renderer.attach(Shader::create("shaders/vertex/player.glsl", "shaders/fragment/player.glsl"));

  if (shader_attached.isLeft()) {
    std::cout << shader_attached.left().message << std::endl;
    throw std::bad_exception();
  };

  o_renderer.spawn(Object::cube(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 0.0f, s_renderer, t_renderer));

  m_o_renderer = o_renderer;
  m_s_renderer = s_renderer;
};

void Player::start() {
  m_o_renderer.start_all();
}

void Player::render() {
  m_o_renderer.render_all();
}
