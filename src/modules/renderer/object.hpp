#pragma once
#include "./shader-renderer.hpp"
#include "./texture-renderer.hpp"
#include "camera.hpp"
#include "glm/glm.hpp"
#include "uniform.hpp"

struct Buffers {
  GLuint VAO;
  GLuint VBO;
};

class Object {
  public:
  Object(
      glm::vec3 position,
      glm::vec3 scale,
      glm::vec3 rotation,
      float angle,
      float *vertices, size_t vertices_size, ShaderRenderer shader_render, TextureRenderer texture_render);

  ShaderRenderer shader_render;
  TextureRenderer texture_render;
  glm::vec3 position;
  glm::vec3 scale;
  glm::vec3 rotation;
  float rotation_angle;

  unsigned int texture;

  glm::mat4 render_transform;

  float *vertices;
  size_t vertices_size;

  static Object cube(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, float angle, ShaderRenderer shader_render, TextureRenderer texture_render);

  Camera camera;

  void start();
  void render();

  private:
  void draw();
  void create_buffers();
  Buffers m_buffers;
  glm::mat4 m_initial_transform;
  Uniform m_uniform;
  glm::vec3 m_initial_position;
};
