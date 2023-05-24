#include "./texture-renderer.hpp"
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "glad/glad.h"
#include "uniform.hpp"

void TextureRenderer::attach(Texture texture) {
  this->textures.push_back(texture);
}

Either<BaseException, Unit> TextureRenderer::attach(Either<BaseException, Texture> texture) {
  if (texture.isLeft()) {
    return texture.left();
  }

  this->textures.push_back(texture.right());

  return Unit();
}

void TextureRenderer::attachMany(size_t size, Texture *textures) {
  for (size_t i = 0; i < size; ++i) {
    attach(textures[i]);
  }
}

Either<BaseException, Unit> TextureRenderer::attachMany(size_t size, Either<BaseException, Texture *> textures) {
  if (textures.isLeft()) {
    return textures.left();
  }

  attachMany(size, textures.right());

  return Unit();
};

void TextureRenderer::start() {
  for (int i = 0; i < textures.size(); ++i) {
    textures[i].uniform.setInt(textures[i].name, i);
  }
}

void TextureRenderer::render() {
  for (int i = 0; i < textures.size(); ++i) {
    glActiveTexture(i == 0 ? GL_TEXTURE0 : GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
  }
}
