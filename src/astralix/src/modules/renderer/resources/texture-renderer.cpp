#include "texture-renderer.hpp"
#include "either.hpp"
#include "engine.hpp"
#include "exceptions/base-exception.hpp"
#include "glad/glad.h"
#include "uniform.hpp"

namespace astralix {

void TextureRenderer::attach(TextureRenderData data) {
  auto resource_manager = ResourceManager::get();
  auto texture_ptr = resource_manager->get_texture_by_id(data.id);

  ASTRA_ASSERT_THROW(texture_ptr == nullptr, "Texture not found");

  m_textures.push_back(data);
}

void TextureRenderer::attach_cubemap(TextureRenderData data) {
  m_cubemaps.push_back(data);
}

void TextureRenderer::attach_many(size_t size, TextureRenderData *textures) {
  for (size_t i = 0; i < size; ++i) {
    attach(textures[i]);
  }
}

void TextureRenderer::start() {}

void TextureRenderer::render(Uniform *uniform) {

  auto resource_manager = ResourceManager::get();

  for (u_int i = 0; i < m_cubemaps.size(); i++) {
    auto cubemap_ptr = resource_manager->get_cubemap_by_id(m_cubemaps[i].id);

    if (cubemap_ptr != nullptr) {
      glActiveTexture(i == 0 ? GL_TEXTURE0 : GL_TEXTURE0 + i);

      uniform->setInt(m_cubemaps[i].name.c_str(), i);

      glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_ptr->get_id());
    }
  }

  for (u_int i = 0; i < m_textures.size(); ++i) {
    auto texture_ptr = resource_manager->get_texture_by_id(m_textures[i].id);

    if (texture_ptr != nullptr) {
      glActiveTexture(i == 0 ? GL_TEXTURE0 : GL_TEXTURE0 + i);

      uniform->setInt(m_textures[i].name.c_str(), i);

      glBindTexture(GL_TEXTURE_2D, texture_ptr->get_id());
    }
  }
}

} // namespace astralix
