#include "./uniform.hpp"
#include "./shader.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Uniform::Uniform() {}

Uniform::Uniform(Shader *shader)
{
  this->shader = shader;
}

void Uniform::setBool(const char *name, bool value) const
{
  glUniform1i(glGetUniformLocation(shader->signed_to, name), (int)value);
};

void Uniform::setInt(const char *name, int value) const
{
  glUniform1i(glGetUniformLocation(shader->signed_to, name), (int)value);
};

void Uniform::setMatrix(const char *name, glm::mat4 trans) const
{
  glUniformMatrix4fv(glGetUniformLocation(shader->signed_to, name), 1, GL_FALSE, glm::value_ptr(trans));
};

void Uniform::setFloat(const char *name, float value) const
{
  glUniform1i(glGetUniformLocation(shader->signed_to, name), value);
};