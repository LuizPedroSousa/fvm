#include "texture.hpp"
#include "assert.hpp"
#include "filesystem"
#include "glad/glad.h"
#include "stb_image/stb_image.h"
#include <cstdarg>
#include <cstring>

namespace astralix {

Texture::Texture(RESOURCE_INIT_PARAMS, unsigned int p_id)
    : RESOURCE_INIT(), m_id(p_id) {}

Either<BaseException, Texture> Texture::create(CreateTextureDTO dto) {

  auto path = get_path(dto.filename);

  auto has_loaded =
      load_image(std::string(dto.filename), dto.flip_image_on_loading);

  ASTRA_ASSERT_EITHER(has_loaded);

  auto image = has_loaded.right();

  unsigned int id;

  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  glGenerateMipmap(GL_TEXTURE_2D);

  // set the texture wrapping/filtering options (on currently bound texture)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  int format = get_image_format(image.nr_channels);

  glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, 0, format,
               GL_UNSIGNED_BYTE, image.data);
  stbi_image_free(image.data);

  return Texture(dto.id, id);
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

Either<BaseException, Texture *>
Texture::create_many(std::initializer_list<CreateTextureDTO> dtos) {
  Texture *result = new Texture[dtos.size()];

  int i = 0;
  for (auto &dto : dtos) {

    auto texture = create(dto);

    ASTRA_ASSERT_EITHER(texture);

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
Texture::create_cubemap(ResourceID resource_id,
                        std::vector<std::string> faces) {
  unsigned int id;
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_CUBE_MAP, id);

  for (u_int i = 0; i < faces.size(); i++) {
    auto has_loaded = load_image(faces[i], false);
    ASTRA_ASSERT_EITHER(has_loaded);

    auto image = has_loaded.right();

    int format = get_image_format(image.nr_channels);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, image.width,
                 image.height, 0, format, GL_UNSIGNED_BYTE, image.data);
    stbi_image_free(image.data);
  }

  // set the texture wrapping/filtering options (on currently bound cube_map)
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return Texture(resource_id, id);
};

std::filesystem::path Texture::get_path(const char *filename) {
  auto base_path = std::filesystem::current_path()
                       .parent_path()
                       .append("src")
                       .append("assets")
                       .append(filename);

  return base_path;
};

Either<BaseException, Image> Texture::load_image(std::string filename,
                                                 bool flip_image_on_loading) {

  int width, height, nr_channels;

  unsigned char *data;

  auto path = get_path(filename.c_str());

  stbi_set_flip_vertically_on_load(flip_image_on_loading);

  data = stbi_load(path.c_str(), &width, &height, &nr_channels, 0);

  if (!data) {
    stbi_image_free(data);
    ASTRA_ASSERT(true, "Cant't load image");
  }

  return Image{width, height, nr_channels, data};
};

} // namespace astralix