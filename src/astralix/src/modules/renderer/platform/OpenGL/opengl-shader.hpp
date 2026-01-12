#pragma once

#include "resources/shader.hpp"

namespace astralix {

class OpenGLShader : public Shader {
public:
  OpenGLShader(const ResourceID &resource_id, const std::string &fragment_path,
               const std::string &vertex_path,
               const std::string &geometry_path = nullptr);

  ~OpenGLShader();

  void bind() const override;
  void unbind() const override;
  void attach() const override;

  void set_bool(const std::string &name, bool value) const override;
  void set_int(const std::string &name, int value) const override;
  void set_matrix(const std::string &name, glm::mat4 matrix) const override;
  void set_float(const std::string &name, float value) const override;
  void set_vec3(const std::string &name, glm::vec3 value) const override;
  uint32_t get_id() const override { return m_renderer_id; };

private:
  uint32_t compile(const std::string &path, uint32_t type);

  uint32_t m_renderer_id = -1;
  uint32_t m_vertex_id = -1;
  uint32_t m_fragment_id = -1;
  uint32_t m_geometry_id = -1;
};
} // namespace astralix
