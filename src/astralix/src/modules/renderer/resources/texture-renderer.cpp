#include "texture-renderer.hpp"
#include "either.hpp"
#include "engine.hpp"
#include "exceptions/base-exception.hpp"

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

void TextureRenderer::render(Ref<Shader> shader) {

  auto resource_manager = ResourceManager::get();

  for (uint32_t slot = 0; slot < m_cubemaps.size(); slot++) {
    auto cubemap_ptr = resource_manager->get_texture_by_id(m_cubemaps[slot].id);

    ASTRA_ASSERT_THROW(cubemap_ptr == nullptr,
                       "TRYING TO RENDER: Texture3D was not found");

    cubemap_ptr->active(slot);

    shader->set_int(m_cubemaps[slot].name.c_str(), slot);

    cubemap_ptr->bind();
  }

  for (uint32_t slot = 0; slot < m_textures.size(); slot++) {
    auto texture_ptr = resource_manager->get_texture_by_id(m_textures[slot].id);

    ASTRA_ASSERT_THROW(texture_ptr == nullptr,
                       "TRYING TO RENDER: Texture2D was not found");

    texture_ptr->active(slot);

    shader->set_int(m_textures[slot].name.c_str(), slot);

    texture_ptr->bind();
  }
}

} // namespace astralix
