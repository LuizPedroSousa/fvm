#include "texture.hpp"
#include "filesystem"
#include "glad/glad.h"
#include "stb_image/stb_image.h"
#include <cstdarg>
#include <cstring>

namespace astralix {

Texture::Texture(RESOURCE_INIT_PARAMS, unsigned int p_id, std::string p_name,
                 int p_width, int p_height, const char *p_path)
    : RESOURCE_INIT(), m_id(p_id), m_name(p_name), m_width(p_width),
      m_height(p_height), m_path(p_path) {}

Texture::Texture(){};

std::filesystem::path Texture::get_path(const char *filename) {
  auto base_path = std::filesystem::current_path()
                       .parent_path()
                       .append("src")
                       .append("assets")
                       .append(filename);

  return base_path;
};

Either<BaseException, Texture> Texture::create(ResourceID resource_id,
                                               std::string name, int color_type,
                                               const char *filename) {
  int width, height, nrChannels;

  auto path = get_path(filename);

  stbi_set_flip_vertically_on_load(true);
  unsigned char *data =
      stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

  ASSERT(!data, "Error loading assets");

  unsigned int id;

  glGenTextures(1, &id);

  glBindTexture(GL_TEXTURE_2D, id);

  // set the texture wrapping/filtering options (on currently bound texture)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  return Texture(resource_id, id, name, width, height, path.c_str());
};

Either<BaseException, Texture *>
Texture::create_many(std::initializer_list<
                     std::tuple<ResourceID, std::string, int, const char *>>
                         textures) {
  Texture *result = new Texture[textures.size()];

  int i = 0;
  for (auto &textureData : textures) {
    auto texture = create(std::get<0>(textureData), std::get<1>(textureData),
                          std::get<2>(textureData), std::get<3>(textureData));

    ASSERT_COMPARE(texture);

    result[i] = texture.right();
    i++;
  }

  return result;
};

} // namespace astralix