#pragma once
#include "glm/glm.hpp"
#include "memory"
#include "utils/guid.hpp"
#include "vector"

struct Buffers {
  u_int VAO;
  u_int VBO;
  u_int EBO;
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

  static Mesh capsule_mesh(float radius, float height, int segments, int rings);
  static Mesh cube_mesh(float size);
  static Mesh plane_mesh(float size);

  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->indices = indices;
  };
  Mesh(){};
};
