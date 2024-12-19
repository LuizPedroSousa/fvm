#include "components/rigidbody/rigidbody-component.hpp"
#include "components/rigidbody/serializers/rigidbody-component-serializer.hpp"
#include "components/transform/transform-component.hpp"
#include "engine.hpp"

namespace astralix {
  RigidBodyComponent::RigidBodyComponent(COMPONENT_INIT_PARAMS, float velocity,
    float gravity, float acceleration,
    float drag, float mass)
    : COMPONENT_INIT(
      RigidBodyComponent, "RigidBody", true,
      create_ref<RigidBodyComponentSerializer>(this)),
    velocity(velocity), gravity(gravity), acceleration(acceleration),
    drag(drag), mass(mass) {

  };

  void RigidBodyComponent::update(float dt) {

    auto transform = get_owner()->get_component<TransformComponent>();

    transform->position.y -= gravity * mass * dt;
  }

}; // namespace astralix
