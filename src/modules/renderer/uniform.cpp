#include "./uniform.hpp"
#include "./shader.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Uniform::Uniform() {}

Uniform::Uniform(unsigned int render_id) {
  m_render_id = render_id;
}

void Uniform::setBool(const char *name, bool value) const {
  glUniform1i(glGetUniformLocation(m_render_id, name), (int)value);
};

void Uniform::setInt(const char *name, int value) const {
  glUniform1i(glGetUniformLocation(m_render_id, name), (int)value);
};

void Uniform::setMatrix(const char *name, glm::mat4 trans) const {
  glUniformMatrix4fv(glGetUniformLocation(m_render_id, name), 1, GL_FALSE, glm::value_ptr(trans));
};

void Uniform::setFloat(const char *name, float value) const {
  glUniform1i(glGetUniformLocation(m_render_id, name), value);
};