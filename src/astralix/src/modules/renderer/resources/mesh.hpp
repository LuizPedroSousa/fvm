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

  static Mesh capsule_mesh(float radius, float height, int segments, int rings);
  static Mesh cube_mesh(float size);
  static Mesh skybox_mesh();
  static Mesh plane_mesh(float size);
  static Mesh quad(float size);

  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->indices  = indices;
  };

  ~Mesh();
  Mesh(){};
};

} // namespace astralix