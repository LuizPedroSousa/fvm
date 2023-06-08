#include "texture-renderer.hpp"
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "game.hpp"
#include "glad/glad.h"
#include "uniform.hpp"

void TextureRenderer::attach(ResourceID texture) {
  m_textures.push_back(texture);
}

void TextureRenderer::attach_many(size_t size, ResourceID *textures) {
  for (size_t i = 0; i < size; ++i) {
    attach(textures[i]);
  }
}

void TextureRenderer::start() {
}

void TextureRenderer::render(Uniform *uniform) {

  auto resource_manager = Game::get()->get_resource_manager();

  for (int i = 0; i < m_textures.size(); ++i) {
    auto texture_ptr = resource_manager->get_texture_by_id(m_textures[i]);

    if (texture_ptr != nullptr) {
      glActiveTexture(i == 0 ? GL_TEXTURE0 : GL_TEXTURE0 + i);

      uniform->setInt(texture_ptr->get_name().c_str(), i);

      glBindTexture(GL_TEXTURE_2D, texture_ptr->get_id());
    }
  }
}
