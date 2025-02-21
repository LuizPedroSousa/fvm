#include "texture.hpp"
#include "assert.hpp"
#include "engine.hpp"
#include "filesystem"
#include "glad/glad.h"
#include "managers/project-manager.hpp"
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
  auto project = ProjectManager::get()->get_active_project();

  auto base_path =
      std::filesystem::path(project->get_config().resources.directory)
          .append(path);

  return base_path;
};

Image Texture::load_image(std::string filename, bool flip_image_on_loading) {

  int width, height, nr_channels;

  unsigned char *data;

  auto path = get_path(filename.c_str());

  stbi_set_flip_vertically_on_load(flip_image_on_loading);

  data = stbi_load(path.c_str(), &width, &height, &nr_channels, 0);

  if (!data) {
    free_image(data);
    ASTRA_EXCEPTION(true, "Cant't load image");
  }

  return Image{width, height, nr_channels, data};
};

Ref<Texture2D> Texture2D::create(
    const ResourceID &resource_id, const std::string &path,
    const bool flip_image_on_loading,
    std::unordered_map<TextureParameter, TextureValue> parameters) {

  return create_renderer_component_ref<Texture2D, OpenGLTexture2D>(
      Engine::get()->renderer_api->get_api(), resource_id,
      TextureConfig{
          .load_image =
              LoadImageConfig{
                  .path = path,
                  .flip_image_on_loading = flip_image_on_loading,
              },
          .parameters = parameters,
      });
};

Ref<Texture2D> Texture2D::create(const ResourceID &resource_id,
                                 TextureConfig config) {
  return create_renderer_component_ref<Texture2D, OpenGLTexture2D>(
      Engine::get()->renderer_api->get_api(), resource_id, config);
};

Ref<Texture3D> Texture3D::create(const ResourceID &resource_id,
                                 const std::vector<std::string> &faces_path) {
  return create_renderer_component_ref<Texture3D, OpenGLTexture3D>(
      Engine::get()->renderer_api->get_api(), resource_id, faces_path);
};

} // namespace astralix
