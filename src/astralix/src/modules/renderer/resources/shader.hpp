#pragma once
#include "base.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "resources/resource.hpp"

namespace astralix {

class Shader : public Resource {
public:
  virtual void bind() const = 0;
  virtual void unbind() const = 0;

  virtual void attach() const = 0;

  virtual void set_bool(const std::string &name, bool value) const = 0;
  virtual void set_int(const std::string &name, int value) const = 0;
  virtual void set_matrix(const std::string &name, glm::mat4 matrix) const = 0;
  virtual void set_float(const std::string &name, float value) const = 0;
  virtual void set_vec3(const std::string &name, glm::vec3 value) const = 0;

  static Ref<Shader> create(const ResourceID &resource_id,
                            const std::string &fragment_path,
                            const std::string &vertex_path,
                            const std::string &geometry_path = "");

protected:
  Shader(const ResourceID &resource_id) : Resource(resource_id) {}

  static std::string get_file(const std::string &path);
};

} // namespace astralix