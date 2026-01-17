#include "resources/texture.hpp"

namespace astralix {

class OpenGLTexture3D : public Texture3D {
public:
  OpenGLTexture3D(const ResourceID &resource_id,
                  const std::vector<Ref<Path>> &face_paths);
  ~OpenGLTexture3D();

  void bind() const override;
  void active(uint32_t slot) const override;
  uint32_t get_renderer_id() const override { return m_renderer_id; };
  uint32_t get_width() const override { return m_width; };
  uint32_t get_height() const override { return m_height; };

private:
  uint32_t m_width;
  uint32_t m_height;
  uint32_t m_renderer_id;
  std::vector<Ref<Path>> m_face_paths;
};

} // namespace astralix
