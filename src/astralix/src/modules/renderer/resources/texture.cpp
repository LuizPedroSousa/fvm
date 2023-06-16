#include "texture.hpp"
#include "filesystem"
#include "glad/glad.h"
#include "stb_image/stb_image.h"
#include <cstdarg>
#include <cstring>

namespace astralix {

Texture::Texture(RESOURCE_INIT_PARAMS, unsigned int p_id, std::string p_name,
                 int p_width, int p_height)
    : RESOURCE_INIT(), m_id(p_id), m_name(p_name), m_width(p_width),
      m_height(p_height) {}

Texture::Texture(){};

Either<BaseException, Texture> Texture::create(ResourceID resource_id,
                                               std::string name,
                                               const char *filename) {
  int width, height, nr_channels;

  unsigned char *data;

  auto path = get_path(filename);

  load_image(std::string(filename), &width, &height, &nr_channels, &data);

  unsigned int id;

  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  glGenerateMipmap(GL_TEXTURE_2D);

  // set the texture wrapping/filtering options (on currently bound texture)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  int format = get_image_format(nr_channels);

  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, data);
  stbi_image_free(data);

  return Texture(resource_id, id, name, width, height);
};

int Texture::get_image_format(int nr_channels) {
  switch (nr_channels) {
  case 1:
    return GL_RED;
  case 3:
    return GL_RGB;

  default:
    return GL_RGBA;
  }
}

Either<BaseException, Texture *> Texture::create_many(
    std::initializer_list<std::tuple<ResourceID, std::string, const char *>>
        textures) {
  Texture *result = new Texture[textures.size()];

  int i = 0;
  for (auto &textureData : textures) {
    auto [id, name, filename] = textureData;

    auto texture = create(id, name, filename);

    ASSERT_COMPARE(texture);

    result[i] = texture.right();
    i++;
  }

  return result;
};

// loads a cubemap texture from 6 individual textures
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
// -------------------------------------------------------
Either<BaseException, Texture>
Texture::create_cubemap(ResourceID resource_id, std::string name,
                        std::vector<std::string> faces) {
  unsigned int id;
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_CUBE_MAP, id);

  int width, height, nr_channels;

  unsigned char *data;

  for (u_int i = 0; i < faces.size(); i++) {
    auto has_loaded =
        load_image(faces[i], &width, &height, &nr_channels, &data);
    ASSERT_COMPARE(has_loaded);

    int format = get_image_format(nr_channels);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height,
                 0, format, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
  }

  // set the texture wrapping/filtering options (on currently bound cube_map)
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return Texture(resource_id, id, name, width, height);
};

std::filesystem::path Texture::get_path(const char *filename) {
  auto base_path = std::filesystem::current_path()
                       .parent_path()
                       .append("src")
                       .append("assets")
                       .append(filename);

  return base_path;
};

Either<BaseException, Unit> Texture::load_image(std::string filename,
                                                int *width, int *height,
                                                int *nr_channels,
                                                unsigned char **data) {
  auto path = get_path(filename.c_str());

  *data = stbi_load(path.c_str(), width, height, nr_channels, 0);

  if (!data) {
    stbi_image_free(data);
    ASSERT(true, "Error loading assets");
  }

  return Unit();
};

} // namespace astralix