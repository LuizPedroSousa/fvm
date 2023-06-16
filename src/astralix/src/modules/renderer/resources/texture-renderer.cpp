#include "texture-renderer.hpp"
#include "either.hpp"
#include "engine.hpp"
#include "exceptions/base-exception.hpp"
#include "glad/glad.h"
#include "uniform.hpp"

namespace astralix {

void TextureRenderer::attach(ResourceID texture_id) {
  m_textures.push_back(texture_id);
}

void TextureRenderer::attach_cubemap(ResourceID cubemap_id) {
  m_cubemaps.push_back(cubemap_id);
}

void TextureRenderer::attach_many(size_t size, ResourceID *textures) {
  for (size_t i = 0; i < size; ++i) {
    attach(textures[i]);
  }
}

void TextureRenderer::start() {}

void TextureRenderer::render(Uniform *uniform) {

  auto resource_manager = Engine::get()->get_resource_manager();

  for (u_int i = 0; i < m_cubemaps.size(); i++) {
    auto texture_ptr = resource_manager->get_cubemap_by_id(m_cubemaps[i]);

    if (texture_ptr != nullptr) {
      glActiveTexture(i == 0 ? GL_TEXTURE0 : GL_TEXTURE0 + i);

      uniform->setInt(texture_ptr->get_name().c_str(), i);

      glBindTexture(GL_TEXTURE_CUBE_MAP, texture_ptr->get_id());
    }
  }

  for (u_int i = 0; i < m_textures.size(); ++i) {
    auto texture_ptr = resource_manager->get_texture_by_id(m_textures[i]);

    if (texture_ptr != nullptr) {
      glActiveTexture(i == 0 ? GL_TEXTURE0 : GL_TEXTURE0 + i);

      uniform->setInt(texture_ptr->get_name().c_str(), i);

      glBindTexture(GL_TEXTURE_2D, texture_ptr->get_id());
    }
  }
}

} // namespace astralix
