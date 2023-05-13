#include "./texture-renderer.hpp"
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "glad/glad.h"

void TextureRenderer::attach(Texture texture)
{
  this->textures.push_back(texture);
}

void TextureRenderer::attachMany(size_t size, Texture *textures)
{
  for (size_t i = 0; i < size; ++i)
  {
    attach(textures[i]);
  }
}

void TextureRenderer::use()
{
  for (int i = 0; i < textures.size(); ++i)
  {
    glActiveTexture(i == 0 ? GL_TEXTURE0 : GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
  }
}
