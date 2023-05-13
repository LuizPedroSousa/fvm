#pragma once
#include "./shader-renderer.hpp"
#include "./texture-renderer.hpp"
#include "glm/glm.hpp"

struct Buffers
{
  GLuint VAO;
  GLuint VBO;
};

class Object
{
private:
  void draw();

public:
  Object(
      glm::vec3 position,
      float *vertices, size_t vertices_size, ShaderRenderer shader_render, TextureRenderer texture_render);
  ShaderRenderer shader_render;
  TextureRenderer texture_render;
  glm::vec3 position;
  Buffers buffers;

  unsigned int texture;

  glm::mat4 init_transform;
  glm::mat4 render_transform;

  float *vertices;
  size_t vertices_size;

  static Object cube(glm::vec3 position, ShaderRenderer shader_render, TextureRenderer texture_render);

  void start();
  void render();
};
