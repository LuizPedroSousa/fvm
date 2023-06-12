#include "mesh.hpp"
#include "glm/glm.hpp"
#include "numbers"

namespace astralix {

Mesh Mesh::plane_mesh(float size) {
  float half_length = size / 2.0f;

  std::vector<Vertex> vertices = {
      // Front face
      {glm::vec3(-half_length, -half_length, half_length),
       glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
      {glm::vec3(half_length, -half_length, half_length),
       glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
      {glm::vec3(half_length, half_length, half_length),
       glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
      {glm::vec3(-half_length, half_length, half_length),
       glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)}};

  std::vector<unsigned int> indices = {
      // Front face
      0, 1, 2, 2, 3, 0,
  };

  return Mesh(vertices, indices);
}

Mesh Mesh::cube_mesh(float size) {
  float half_length = size / 2.0f;

  std::vector<Vertex> vertices = {
      // Front face
      {glm::vec3(-half_length, -half_length, half_length),
       glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
      {glm::vec3(half_length, -half_length, half_length),
       glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
      {glm::vec3(half_length, half_length, half_length),
       glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
      {glm::vec3(-half_length, half_length, half_length),
       glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},

      // Back face
      {glm::vec3(half_length, -half_length, -half_length),
       glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
      {glm::vec3(-half_length, -half_length, -half_length),
       glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
      {glm::vec3(-half_length, half_length, -half_length),
       glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
      {glm::vec3(half_length, half_length, -half_length),
       glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},

      // Left face
      {glm::vec3(-half_length, -half_length, -half_length),
       glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
      {glm::vec3(-half_length, -half_length, half_length),
       glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
      {glm::vec3(-half_length, half_length, half_length),
       glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
      {glm::vec3(-half_length, half_length, -half_length),
       glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},

      // Right face
      {glm::vec3(half_length, -half_length, half_length),
       glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
      {glm::vec3(half_length, -half_length, -half_length),
       glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
      {glm::vec3(half_length, half_length, -half_length),
       glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
      {glm::vec3(half_length, half_length, half_length),
       glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},

      // Top face
      {glm::vec3(-half_length, half_length, half_length),
       glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
      {glm::vec3(half_length, half_length, half_length),
       glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
      {glm::vec3(half_length, half_length, -half_length),
       glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
      {glm::vec3(-half_length, half_length, -half_length),
       glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},

      // Bottom face
      {glm::vec3(-half_length, -half_length, -half_length),
       glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
      {glm::vec3(half_length, -half_length, -half_length),
       glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
      {glm::vec3(half_length, -half_length, half_length),
       glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
      {glm::vec3(-half_length, -half_length, half_length),
       glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)}};

  std::vector<unsigned int> indices = {// Front face
                                       0, 1, 2, 2, 3, 0,

                                       // Back face
                                       4, 5, 6, 6, 7, 4,

                                       // Left face
                                       8, 9, 10, 10, 11, 8,

                                       // Right face
                                       12, 13, 14, 14, 15, 12,

                                       // Top face
                                       16, 17, 18, 18, 19, 16,

                                       // Bottom face
                                       20, 21, 22, 22, 23, 20};

  return Mesh(vertices, indices);
}

Mesh Mesh::quad(float size) {
  std::vector<Vertex> vertices = {
      {glm::vec3(-size, size, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)},
      {glm::vec3(-size, -size, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)},
      {glm::vec3(size, -size, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)},
      {glm::vec3(size, size, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)},
  };

  std::vector<unsigned int> indices = {0, 1, 2, 2,
                                       3, 0

  };

  return Mesh(vertices, indices);
}

Mesh Mesh::capsule_mesh(float radius, float height, int segments, int rings) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  const float pi = std::numbers::pi_v<float>;

  for (int ring = 0; ring <= rings; ++ring) {
    float phi = ring * pi / rings;
    float y = height * (ring / static_cast<float>(rings)) - height * 0.5f;

    for (int segment = 0; segment <= segments; ++segment) {
      float theta = segment * 2 * pi / segments;
      float x = radius * std::cos(theta) * std::sin(phi);
      float z = radius * std::sin(theta) * std::sin(phi);

      Vertex vertex;
      vertex.position = glm::vec3(x, y, z);

      vertices.push_back(vertex);
    }
  }

  for (int ring = 0; ring < rings; ++ring) {
    for (int segment = 0; segment < segments; ++segment) {
      int current_ring = ring * (segments + 1);
      int next_ring = (ring + 1) * (segments + 1);

      indices.push_back(current_ring + segment);
      indices.push_back(next_ring + segment);
      indices.push_back(next_ring + segment + 1);

      indices.push_back(current_ring + segment);
      indices.push_back(next_ring + segment + 1);
      indices.push_back(current_ring + segment + 1);
    }
  }

  return Mesh(vertices, indices);
}
} // namespace astralix