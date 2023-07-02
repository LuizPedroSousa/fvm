#include "mesh.hpp"
#include "glm/glm.hpp"
#include "iostream"
#include "numbers"

namespace astralix {

Mesh Mesh::plane(float size) {
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

Mesh Mesh::cube(float size) {
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

Mesh Mesh::sphere() {

  const int segments = 32;
  const int rings = 16;
  const float radius = 0.5f;

  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  const float pi = std::numbers::pi_v<float>;

  for (int ring = 0; ring <= rings; ++ring) {
    float phi = ring * pi / rings;
    float y = radius * std::cos(phi);

    for (int segment = 0; segment <= segments; ++segment) {
      float theta = segment * 2 * pi / segments;
      float x = radius * std::sin(phi) * std::cos(theta);
      float z = radius * std::sin(phi) * std::sin(theta);

      Vertex vertex;
      vertex.position = glm::vec3(x, y, z);
      vertex.normal = glm::normalize(vertex.position);
      vertex.texture_cordinates =
          glm::vec2(static_cast<float>(segment) / segments,
                    static_cast<float>(ring) / rings);

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

Mesh Mesh::capsule(float radius, float height, int segments, int rings) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  const float pi = std::numbers::pi_v<float>;
  const float halfHeight = height * 0.5f;

  for (int ring = 0; ring <= rings; ++ring) {
    float phi = ring * pi / rings;
    float y = std::cos(phi);
    float ringRadius = std::sin(phi);

    for (int segment = 0; segment <= segments; ++segment) {
      float theta = segment * 2 * pi / segments;
      float x = std::cos(theta) * ringRadius;
      float z = std::sin(theta) * ringRadius;

      Vertex vertex;
      vertex.position =
          glm::vec3(x * radius, y * radius * halfHeight, z * radius);
      vertex.normal = glm::normalize(vertex.position);
      vertex.texture_cordinates =
          glm::vec2(static_cast<float>(segment) / segments,
                    static_cast<float>(ring) / rings);

      vertices.push_back(vertex);
    }
  }

  for (int ring = 0; ring < rings; ++ring) {
    for (int segment = 0; segment < segments; ++segment) {
      int currentRing = ring * (segments + 1);
      int nextRing = (ring + 1) * (segments + 1);

      indices.push_back(currentRing + segment);
      indices.push_back(nextRing + segment);
      indices.push_back(nextRing + segment + 1);

      indices.push_back(currentRing + segment);
      indices.push_back(nextRing + segment + 1);
      indices.push_back(currentRing + segment + 1);
    }
  }

  return Mesh(vertices, indices);
}

} // namespace astralix