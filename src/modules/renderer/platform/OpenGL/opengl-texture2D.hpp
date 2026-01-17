#pragma once

#include "resources/texture.hpp"

namespace astralix {
class OpenGLTexture2D : public Texture2D {
public:
  OpenGLTexture2D(const ResourceID &resource_id, TextureConfig config);

  static Ref<Texture2D> create(const ResourceID &resource_id);

  ~OpenGLTexture2D();

  void bind() const override;
  void active(uint32_t slot) const override;
  uint32_t get_renderer_id() const override { return m_renderer_id; };
  uint32_t get_width() const override { return m_width; };
  uint32_t get_height() const override { return m_height; };

private:
  uint32_t m_width;
  uint32_t m_height;
  uint32_t m_renderer_id;
  int m_format;
  unsigned char *m_buffer;

  unsigned int textureParameterToGL(TextureParameter param);

  int textureParameterValueToGL(TextureValue value);

  int formatToGl(TextureFormat format);
};
} // namespace astralix
