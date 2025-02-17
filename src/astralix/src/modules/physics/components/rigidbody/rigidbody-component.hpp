#pragma once
#include "PxPhysics.h"
#include "PxSceneDesc.h"
#include "ecs/components/component.hpp"

namespace astralix {

enum RigidType { Static = 0, Dynamic = 1 };

class RigidBodyComponent : public Component<RigidBodyComponent> {
public:
  RigidBodyComponent(COMPONENT_INIT_PARAMS,
                     RigidType rigid_type = RigidType::Dynamic,
                     float gravity = 0.5f, float velocity = 2.0f,
                     float acceleration = 2.0f, float drag = 0.0f,
                     float mass = 1.0f);

  void start(physx::PxPhysics *physics, physx::PxScene *scene);
  void update(float dt);

private:
  float velocity;
  float gravity;
  float drag;
  float mass;
  float acceleration;
  RigidType m_rigid_type;
};

} // namespace astralix
