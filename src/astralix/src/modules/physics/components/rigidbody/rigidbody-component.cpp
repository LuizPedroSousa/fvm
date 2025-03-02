#include "components/rigidbody/rigidbody-component.hpp"
#include "PxRigidActor.h"
#include "PxRigidDynamic.h"
#include "components/mesh-collision/mesh-collision-component.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/rigidbody/serializers/rigidbody-component-serializer.hpp"
#include "components/transform/transform-component.hpp"
#include "engine.hpp"
#include "glm/glm.hpp"

#include "utils/math.hpp"

#include "PxPhysics.h"

#include "PxPhysicsAPI.h"

using namespace physx;

namespace astralix {
RigidBodyComponent::RigidBodyComponent(COMPONENT_INIT_PARAMS,
                                       RigidType rigid_type, float velocity,
                                       float gravity, float acceleration,
                                       float drag, float mass)
    : COMPONENT_INIT(RigidBodyComponent, "rigid-body", true,
                     create_ref<RigidBodyComponentSerializer>(this)),
      m_rigid_type(rigid_type), velocity(velocity), gravity(gravity),
      acceleration(acceleration), drag(drag), mass(mass) {

      };

void RigidBodyComponent::start(physx::PxPhysics *physics,
                               physx::PxScene *scene) {
  auto owner = get_owner();

  auto transform = owner->get_component<TransformComponent>();
  auto collision = owner->get_component<MeshCollisionComponent>();

  if (transform == nullptr) {
    return;
  }

  PxQuat quaterion = GlmQuatToPxQuat(glm::quat(transform->rotation));

  PxTransform physics_transform =
      PxTransform(PxVec3(transform->position.x, transform->position.y,
                         transform->position.z),
                  quaterion);

  PxRigidActor *body;

  switch (m_rigid_type) {

  case RigidType::Dynamic: {
    auto dynamic = physics->createRigidDynamic(physics_transform);

    PxRigidBodyExt::updateMassAndInertia(*dynamic, 10.0f);

    body = dynamic;

    break;
  }
  case RigidType::Static: {
    body = physics->createRigidStatic(physics_transform);

    break;
  }
  }

  if (collision != nullptr) {
    collision->attach_shape(body, transform->matrix, physics);
  }

  body->userData = owner;

  scene->addActor(*body);
}

void RigidBodyComponent::update(float dt) {
  // auto transform = get_owner()->get_component<TransformComponent>();

  // transform->position.y -= gravity * mass * dt;
}

}; // namespace astralix
