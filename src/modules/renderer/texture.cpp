#include "./texture.hpp"
#include "filesystem"
#include "glad/glad.h"
#include "stb_image/stb_image.h"
#include <cstdarg>
#include <cstring>

Texture::Texture(unsigned int id, const char *name, int width, int height, Shader shader) : id(id), name(name), width(width), height(height), shader(shader){};
Texture::Texture(){};

std::filesystem::path Texture::getPath(const char *filename) {
  auto base_path = std::filesystem::current_path().parent_path().append("src/assets/textures").append(filename);

  return base_path;
};

Either<BaseException, Texture> Texture::create(const char *name, const char *filename, int color_type, Shader shader) {
  int width, height, nrChannels;

  auto path = getPath(filename);

  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

  if (!data) {
    return BaseException("Error loading assets");
  }

  unsigned int texture_id;

  glGenTextures(1, &texture_id);

  glBindTexture(GL_TEXTURE_2D, texture_id);
  // set the texture wrapping/filtering options (on currently bound texture)

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, color_type, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  return Texture(texture_id, name, width, height, shader);
};

Either<BaseException, Texture *> Texture::createMany(size_t size, std::initializer_list<std::tuple<const char *, const char *, int, Shader>> texturesData) {
  Texture *result = new Texture[size];

  size_t i = 0;

  for (const auto &textureData : texturesData) {
    auto texture = create(std::get<0>(textureData), std::get<1>(textureData), std::get<2>(textureData), std::get<3>(textureData));

    if (texture.isLeft()) {
      return texture.left();
    }

    result[i] = texture.right();

    i++;
  }

  return result;
};
