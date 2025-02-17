#pragma once
#include "ecs/guid.hpp"
#include "glm/glm.hpp"
#include "memory"
#include "renderer-api.hpp"
#include "vector"
#include "vertex-array.hpp"

namespace astralix {

class Mesh;

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texture_cordinates;
};

class Mesh {
public:
  Ref<VertexArray> vertex_array;

  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  static Mesh capsule(float radius = 0.5f, float height = 1.0f,
                      int segments = 16, int rings = 8);

  static Mesh cube(float size = 2.0f);
  static Mesh plane(float size = 2.0f);
  static Mesh quad(float size = 1.0f);
  static Mesh sphere();

  RendererAPI::DrawPrimitiveType draw_type =
      RendererAPI::DrawPrimitiveType::TRIANGLES;

  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->indices = indices;
  };

  ~Mesh() = default;
};

} // namespace astralix
