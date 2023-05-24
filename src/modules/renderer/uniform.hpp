#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "string"

class Shader;

class Uniform {
  private:
  unsigned int m_render_id;

  public:
  Uniform();
  Uniform(unsigned int render_id);
  void setBool(const char *name, bool value) const;
  void setInt(const char *name, int value) const;
  void setMatrix(const char *name, glm::mat4 trans) const;
  void setFloat(const char *name, float value) const;
  void setVec3(const char *name, glm::vec3 value) const;
};