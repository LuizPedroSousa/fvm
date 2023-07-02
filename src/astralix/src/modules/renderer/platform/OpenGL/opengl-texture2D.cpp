#include "opengl-texture2D.hpp"
#include "glad/glad.h"

namespace astralix {

OpenGLTexture2D::OpenGLTexture2D(const ResourceID &resource_id,
                                 const std::string &path,
                                 const bool flip_image_on_loading)
    : Texture2D(resource_id), m_path(path) {
  auto absolute_path = this->get_path(path);

  auto image = load_image(absolute_path, flip_image_on_loading);

  this->m_width = image.width;
  this->m_height = image.height;

  glGenTextures(1, &m_renderer_id);
  glBindTexture(GL_TEXTURE_2D, m_renderer_id);

  glGenerateMipmap(GL_TEXTURE_2D);

  // set the texture wrapping/filtering options (on currently bound texture)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  int format = get_image_format(image.nr_channels);

  glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, 0, format,
               GL_UNSIGNED_BYTE, image.data);
  free_image(image.data);
}

void OpenGLTexture2D::bind() const {
  glBindTexture(GL_TEXTURE_2D, m_renderer_id);
}

void OpenGLTexture2D::active(uint32_t slot) const {
  glActiveTexture(slot == 0 ? GL_TEXTURE0 : GL_TEXTURE0 + slot);
}

OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &m_renderer_id); }

} // namespace astralix
