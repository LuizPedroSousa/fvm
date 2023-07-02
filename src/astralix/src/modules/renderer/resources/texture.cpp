#include "texture.hpp"
#include "assert.hpp"
#include "engine.hpp"
#include "filesystem"
#include "glad/glad.h"
#include "platform/OpenGL/opengl-texture2D.hpp"
#include "platform/OpenGL/opengl-texture3D.hpp"
#include "stb_image/stb_image.h"
#include <cstdarg>
#include <cstring>

namespace astralix {

void Texture::free_image(u_char *data) { stbi_image_free(data); }

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

std::filesystem::path Texture::get_path(const std::string &path) {
  return std::filesystem::current_path()
      .parent_path()
      .append("src")
      .append("assets")
      .append(path.c_str());
};

Image Texture::load_image(std::string filename, bool flip_image_on_loading) {

  int width, height, nr_channels;

  unsigned char *data;

  auto path = get_path(filename.c_str());

  stbi_set_flip_vertically_on_load(flip_image_on_loading);

  data = stbi_load(path.c_str(), &width, &height, &nr_channels, 0);

  if (!data) {
    free_image(data);
    ASTRA_ASSERT_THROW(true, "Cant't load image");
  }

  return Image{width, height, nr_channels, data};
};

Ref<Texture2D> Texture2D::create(const ResourceID &resource_id,
                                 const std::string &path,
                                 const bool flip_image_on_loading) {
  return create_renderer_component_ref<Texture2D, OpenGLTexture2D>(
      Engine::get()->renderer_api->get_api(), resource_id, path,
      flip_image_on_loading);
};

Ref<Texture3D> Texture3D::create(const ResourceID &resource_id,
                                 const std::vector<std::string> &faces_path) {
  return create_renderer_component_ref<Texture3D, OpenGLTexture3D>(
      Engine::get()->renderer_api->get_api(), resource_id, faces_path);
};

} // namespace astralix