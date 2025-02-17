#pragma once
#include "PxActor.h"
#include "PxRigidActor.h"
#include "array"
#include "ecs/components/component.hpp"
#include "glm/glm.hpp"
#include "vector"

namespace astralix {

class Simplex {

public:
  Simplex()
      : m_points({glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec3(0)}),
        m_size(0) {

        };

  Simplex &operator=(std::initializer_list<glm::vec3> list) {
    for (auto point = list.begin(); point != list.end(); point++) {
      m_points[std::distance(list.begin(), point)] = *point;
    }

    m_size = list.size();

    return *this;
  }

  glm::vec3 &operator[](unsigned int index) { return m_points[index]; }

  void push_front(glm::vec3 point) {
    m_points = {point, m_points[0], m_points[1], m_points[2]};

    m_size = std::min(m_size + 1, 4u);
  }

  auto size() const { return m_size; }

  auto begin() const { return m_points.begin(); }

  auto end() const { return m_points.end() - (4 - m_size); }

  enum SimplexType { line, triangle, tetrahedron };

  SimplexType get_type() {
    switch (m_size) {
    case 2:
      return SimplexType::line;

    case 3:
      return SimplexType::triangle;

    default:
      return SimplexType::tetrahedron;
    };
  }

private:
  std::array<glm::vec3, 4> m_points;
  unsigned m_size;
};

class MeshCollisionComponent : public Component<MeshCollisionComponent> {
public:
  MeshCollisionComponent(COMPONENT_INIT_PARAMS);

  void start();

  void attach_shape(physx::PxRigidActor *body, glm::mat4 transform_matrix,
                    physx::PxPhysics *physics);

  bool aabb_intersect(MeshCollisionComponent *other);
  bool has_intersect(MeshCollisionComponent *other);

  glm::vec3 get_support(MeshCollisionComponent *other_collider,
                        glm::vec3 direction);
  glm::vec3 find_furthest_point(glm::vec3 direction,
                                std::vector<glm::vec3> vertices);

  bool can_check_next_simplex(Simplex &simplex, glm::vec3 &direction);

  bool search_line(Simplex &simplex, glm::vec3 &direction);
  bool search_triangle(Simplex &simplex, glm::vec3 &direction);
  bool search_tetrahedron(Simplex &simplex, glm::vec3 &direction);
  bool is_same_direction(glm::vec3 &direction, glm::vec3 &ao);

  std::vector<glm::vec3> vertices;
};

} // namespace astralix
