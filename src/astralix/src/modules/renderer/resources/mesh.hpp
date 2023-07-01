#pragma once
#include "ecs/guid.hpp"
#include "glm/glm.hpp"
#include "memory"
#include "vector"

namespace astralix {

struct Buffers {
  u_int VAO = 0;
  u_int VBO = 0;
  u_int EBO = 0;
};

class Mesh;

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texture_cordinates;
};

class Mesh {
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  Buffers m_buffers;

  static Mesh capsule(float radius = 0.5f, float height = 1.0f,
                      int segments = 16, int rings = 8);

  static Mesh cube(float size = 2.0f);
  static Mesh plane(float size = 2.0f);
  static Mesh quad(float size = 1.0f);
  static Mesh sphere();

  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->indices = indices;
  };

  ~Mesh();
  Mesh(){};
};

} // namespace astralix