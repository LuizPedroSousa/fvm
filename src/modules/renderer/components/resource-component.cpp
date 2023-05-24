#include "resource-component.hpp"

ResourceComponent::ResourceComponent(COMPONENT_INIT_PARAMS) : COMPONENT_INIT(ResourceComponent){};

void ResourceComponent::start() {
  m_texture_renderer.start();
  m_shader_renderer.use();
}

void ResourceComponent::update() {
  m_texture_renderer.render();
  m_shader_renderer.use();
}

Either<BaseException, Unit> ResourceComponent::load_texture(Either<BaseException, Texture> texture) {
  ASSERT_COMPARE(texture);

  auto attached = m_texture_renderer.attach(texture);

  ASSERT_COMPARE(attached);

  return Unit();
};

Either<BaseException, Unit> ResourceComponent::load_shader(Either<BaseException, Shader> shader) {
  ASSERT_COMPARE(shader);

  auto attached = m_shader_renderer.attach(shader);

  ASSERT_COMPARE(attached);

  return Unit();
};