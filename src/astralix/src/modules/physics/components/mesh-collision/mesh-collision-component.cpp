#include "mesh-collision-component.hpp"
#include "PxActor.h"
#include "PxPhysics.h"
#include "PxRigidActor.h"
#include "components/mesh-collision/serializers/mesh-collision-component-serializer.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/transform/transform-component.hpp"
#include "engine.hpp"
#include "geometry/PxBoxGeometry.h"
#include "glm/fwd.hpp"
#include "utils/math.hpp"

namespace astralix {

MeshCollisionComponent::MeshCollisionComponent(COMPONENT_INIT_PARAMS)
    : COMPONENT_INIT(MeshCollisionComponent, "Mesh Collision", true,
                     create_ref<MeshCollisionComponentSerializer>(this)) {};

void MeshCollisionComponent::start() {}

inline physx::PxShape *create_box_shape(float half_width, float half_height,
                                        float half_depth,
                                        physx::PxPhysics *physics) {
  auto material = physics->createMaterial(0.5f, 0.5f, 0.6f);
  physx::PxShape *shape = physics->createShape(
      physx::PxBoxGeometry(half_width, half_height, half_depth), *material,
      true);

  return shape;
}

void MeshCollisionComponent::attach_shape(physx::PxRigidActor *body,
                                          glm::mat4 transform_matrix,
                                          physx::PxPhysics *physics) {

  auto transform = get_owner()->get_component<TransformComponent>();

  auto mesh_component = get_owner()->get_component<MeshComponent>();

  if (!mesh_component) {
    return;
  }

  glm::vec3 min_bounds(FLT_MAX);
  glm::vec3 max_bounds(-FLT_MAX);

  for (const auto &mesh : mesh_component->get_meshes()) {
    for (const auto &vertex : mesh.vertices) {
      min_bounds = glm::min(min_bounds, vertex.position);
      max_bounds = glm::max(max_bounds, vertex.position);
    }
  }

  glm::vec3 extents = (max_bounds - min_bounds) * 0.5f * transform->scale;

  glm::vec3 center = (max_bounds + min_bounds) * 0.5f;

  auto shape = create_box_shape(extents.x, extents.y, extents.z, physics);

  physx::PxTransform localPose(GlmVec3ToPxVec3(center));
  shape->setLocalPose(localPose);

  body->attachShape(*shape);

  shape->release();
}

glm::vec3
MeshCollisionComponent::get_support(MeshCollisionComponent *other_collider,
                                    glm::vec3 direction) {

  // vertex minkowski difference
  return find_furthest_point(direction, vertices) -
         other_collider->find_furthest_point(-direction,
                                             other_collider->vertices);
};

glm::vec3
MeshCollisionComponent::find_furthest_point(glm::vec3 direction,
                                            std::vector<glm::vec3> vertices) {
  glm::vec3 max_point;
  float max_distance = -FLT_MAX;

  for (glm::vec3 vertex : vertices) {
    float distance = glm::dot(vertex, direction);

    if (distance > max_distance) {
      max_distance = distance;
      max_point = vertex;
    }
  }

  return max_point;
}

bool MeshCollisionComponent::aabb_intersect(MeshCollisionComponent *other) {
  auto position = get_owner()->get_component<TransformComponent>()->position;

  auto other_position =
      other->get_owner()->get_component<TransformComponent>()->position;

  glm::vec3 min_point, max_point;
  glm::vec3 other_min_point, other_max_point;

  min_point = glm::min(position, 2.0f);
  max_point = glm::max(position, 2.0f);

  other_min_point = glm::min(other_position, 2.0f);
  other_max_point = glm::max(other_position, 2.0f);

  if (max_point.x > other_min_point.x && min_point.x < other_max_point.x &&
      max_point.y > other_min_point.y && min_point.y < other_max_point.y &&
      max_point.z > other_min_point.z && min_point.z < other_max_point.z) {
    return false;
  }

  return true;
}

bool MeshCollisionComponent::has_intersect(MeshCollisionComponent *other) {
  glm::vec3 direction(1.0f, 0.0f, 0.0f);

  auto support = get_support(other, direction);

  Simplex simplex;

  simplex.push_front(support);

  direction = -support;

  while (true) {
    support = get_support(other, direction);
    auto point_exists = glm::dot(direction, support);

    if (point_exists <= 0) {
      return false;
    }

    simplex.push_front(support);

    if (can_check_next_simplex(simplex, direction)) {
      return true;
    }
  }
};

bool MeshCollisionComponent::search_line(Simplex &simplex,
                                         glm::vec3 &direction) {
  glm::vec3 a = simplex[0];
  glm::vec3 b = simplex[1];

  glm::vec3 ab = b - a;

  glm::vec3 ao = -a;

  if (is_same_direction(ab, ao)) {
    direction = glm::cross(glm::cross(ab, ao), b);
  } else {
    simplex = {a};
    direction = ao;
  };

  return false;
};

bool MeshCollisionComponent::search_triangle(Simplex &simplex,
                                             glm::vec3 &direction) {
  glm::vec3 a = simplex[0];
  glm::vec3 b = simplex[1];
  glm::vec3 c = simplex[2];

  glm::vec3 ab = b - a;
  glm::vec3 ac = c - a;

  glm::vec3 ao = -a;

  glm::vec3 abc = glm::cross(ab, ac);

  glm::vec3 abc_ac = glm::cross(abc, ac);

  if (is_same_direction(abc_ac, ao)) {
    if (is_same_direction(ac, ao)) {
      simplex = {a, c};

      direction = glm::cross(glm::cross(ac, ao), ac);
    } else {
      simplex = {a, b};
      return search_line(simplex, direction);
    }
  } else {
    auto ab_abc = glm::cross(ab, abc);
    if (is_same_direction(ab_abc, ao)) {
      simplex = {a, b};
      return search_line(simplex, direction);
    } else if (is_same_direction(abc, ao)) {
      direction = abc;
    } else {
      simplex = {a, c, b};
      direction = -abc;
    }
  }

  return false;
};

bool MeshCollisionComponent::search_tetrahedron(Simplex &simplex,
                                                glm::vec3 &direction) {
  glm::vec3 a = simplex[0];
  glm::vec3 b = simplex[1];
  glm::vec3 c = simplex[2];
  glm::vec3 d = simplex[3];

  glm::vec3 ab = b - a;
  glm::vec3 ac = c - a;
  glm::vec3 ad = d - a;

  glm::vec3 ao = -a;

  glm::vec3 abc = glm::cross(ab, ac);
  glm::vec3 acd = glm::cross(ac, ad);
  glm::vec3 adb = glm::cross(ad, ab);

  if (is_same_direction(abc, ao)) {
    simplex = {a, b, c};

    return search_triangle(simplex, direction);
  }

  if (is_same_direction(acd, ao)) {
    simplex = {a, c, d};

    return search_triangle(simplex, direction);
  }

  if (is_same_direction(adb, ao)) {
    simplex = {a, d, b};

    return search_triangle(simplex, direction);
  }

  return true;
};

bool MeshCollisionComponent::is_same_direction(glm::vec3 &direction,
                                               glm::vec3 &ao) {
  return glm::dot(direction, ao) > 0;
};

bool MeshCollisionComponent::can_check_next_simplex(Simplex &simplex,
                                                    glm::vec3 &direction) {

  switch (simplex.get_type()) {
  case Simplex::line:
    return search_line(simplex, direction);

  case Simplex::triangle:

    return search_triangle(simplex, direction);
  case Simplex::tetrahedron:

    return search_tetrahedron(simplex, direction);
  }

  return false;
}

} // namespace astralix
