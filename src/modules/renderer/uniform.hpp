#pragma once
#include "string"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader;

class Uniform
{
public:
  Shader *shader;
  Uniform();
  Uniform(Shader *shader);
  void setBool(const char *name, bool value) const;
  void setInt(const char *name, int value) const;
  void setMatrix(const char *name, glm::mat4 trans) const;
  void setFloat(const char *name, float value) const;
};