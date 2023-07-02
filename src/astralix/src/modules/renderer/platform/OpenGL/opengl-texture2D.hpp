#pragma once

#include "resources/texture.hpp"

namespace astralix {
class OpenGLTexture2D : public Texture2D {
public:
  OpenGLTexture2D(const ResourceID &resource_id, const std::string &path,
                  const bool flip_image_on_loading = false);

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
  std::string m_path;
};
} // namespace astralix